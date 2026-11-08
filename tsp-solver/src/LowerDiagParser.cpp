#include "TspInstance.hpp"
#include "TspParser.hpp"
#include "MatrixGraph.hpp"
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>



namespace tsp {

TspInstance TspParser::parseLowerDiag(std::istream& input) {
    std::vector<int> cityIds(dimension);

    DistanceMatrix distances(dimension, std::vector<double>(dimension, 0.0));

    for (int i = 0; i < dimension; i++) {
        cityIds[i] = i + 1;
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j <= i; j++) {
            int weight;

            if (!(input >> weight)) {
                throw std::runtime_error("Missing or invalid integer distance");
            }

            if (weight < 0) {
                throw std::runtime_error("Distances cannot be negative");
            }

            if (i == j && weight != 0) {
                throw std::runtime_error("The distance from a city to itself must be zero");
            }

            distances[i][j] = weight;
            distances[j][i] = weight;
        }
    }

    // dantzig42 has a DISPLAY_DATA_SECTION (drawing coordinates) after the matrix: skip it
    std::string keyword;

    if (input >> keyword && keyword == "DISPLAY_DATA_SECTION") {
        while (input >> keyword && keyword != "EOF") {}
    }

    if (input && keyword != "EOF") {
        throw std::runtime_error("Expected EOF after the matrix distances");
    }

    std::string extra;

    if (input >> extra) {
        throw std::runtime_error("Unexpected data after EOF");
    }

    if (input.bad()) {
        throw std::runtime_error("An error occurred while reading the file");
    }

    return TspInstance(name, std::move(cityIds), std::make_unique<MatrixGraph>(std::move(distances)));
}

}
