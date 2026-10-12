#include "TspParser.hpp"
#include "CoordinatesGraph.hpp"
#include <stdexcept>
#include <utility>
#include <vector>

namespace tsp {

TspInstance TspParser::parseCoordinates(std::istream& input) {
    // Create the data structures we need
    std::vector<int> cityIds(dimension);
    std::vector<std::pair<double, double>> coordinates(dimension);

    // Read every city: id x y
    for (int i = 0; i < dimension; ++i) {
        int id = 0;
        double x = 0.0;
        double y = 0.0;

        if (!(input >> id >> x >> y)) {
            throw std::runtime_error("Missing coordinate data.");
        }

        if (id < 1 || id > dimension) {
            throw std::runtime_error("City id out of range.");
        }

        if (cityIds[id - 1] != 0) {
            throw std::runtime_error("Duplicate city id.");
        }

        cityIds[id - 1] = id;
        coordinates[id - 1] = std::make_pair(x, y);
    }

    // Some files end with EOF after the coordinates
    std::string extra;
    if (input >> extra) {
        if (extra != "EOF") {
            throw std::runtime_error("Unexpected data after coordinate section.");
        }
    }

    if (input.bad()) {
        throw std::runtime_error("Error while reading coordinates.");
    }

    // Build the graph and return the instance
    return TspInstance(
        name,
        std::move(cityIds),
        std::make_unique<CoordinatesGraph>(std::move(coordinates))
    );
}

}