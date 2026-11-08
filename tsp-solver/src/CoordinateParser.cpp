#include "TspParser.hpp"
#include "CoordinatesGraph.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


namespace tsp {

TspInstance TspParser::parseCoordinates(std::istream& input) {
    std::vector<int> ids(dimension);
    std::vector<std::pair<double, double>> points(dimension);

    for (int i = 0; i < dimension; i++) {
        if (!(input >> ids[i] >> points[i].first >> points[i].second)) {
            throw std::runtime_error("Missing coordinate data");
        }
    }

    // ids start at 1 in TSPLIB files and at 0 in files from instance_generator.py (so we need to check minimu)
    const int firstId = (*std::min_element(ids.begin(), ids.end()) == 0) ? 0 : 1;

    // city k is stored at index k - firstId, whatever the order of the lines
    std::vector<int> cityIds(dimension, -1);
    std::vector<std::pair<double, double>> coordinates(dimension);

    for (int i = 0; i < dimension; ++i) {
        const int index = ids[i] - firstId;

        if (index < 0 || index >= dimension) {
            throw std::runtime_error("City id out of range");
        }

        if (cityIds[index] != -1) {
            throw std::runtime_error("Duplicate city id");
        }

        cityIds[index] = ids[i];
        coordinates[index] = points[i];
    }

    std::string extra;
    if (input >> extra) {
        if (extra != "EOF") {
            throw std::runtime_error("Unexpected data after coordinate section");
        }

        if (input >> extra) {
            throw std::runtime_error("Unexpected data after EOF");
        }
    }

    if (input.bad()) {
        throw std::runtime_error("Error while reading coordinates");
    }

    return TspInstance(name, std::move(cityIds), std::make_unique<CoordinatesGraph>(std::move(coordinates)));
}

}
