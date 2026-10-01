#include "TspInstance.hpp"
#include "TspParser.hpp"
#include <istream>
#include <stdexcept>


namespace tsp {

TspInstance TspParser::parseLowerDiag(std::istream& input) {
    TspInstance instance;
    instance.name = name;

    instance.cityIds.resize(dimension);
    instance.distances.assign(
        dimension,
        std::vector<double>(dimension, 0.0)
    );

    for (int i = 0; i < dimension; i++) {
        instance.cityIds[i] = i + 1;
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j <= i; ++j) {
            int weight;

            if (!(input >> weight)) {
                throw std::runtime_error(
                    "Missing or invalid integer distance."
                );
            }

            if (weight < 0) {
                throw std::runtime_error(
                    "Distances cannot be negative."
                );
            }

            if (i == j && weight != 0) {
                throw std::runtime_error(
                    "The distance from a city to itself must be zero."
                );
            }

            instance.distances[i][j] = weight;
            instance.distances[j][i] = weight;
        }
    }

    std::string endMarker;

    if (!(input >> endMarker) || endMarker != "EOF") {
        throw std::runtime_error(
            "Expected EOF after the matrix distances."
        );
    }

    std::string extra;

    if (input >> extra) {
        throw std::runtime_error(
            "Unexpected data after EOF."
        );
    }

    if (input.bad()) {
        throw std::runtime_error(
            "An error occurred while reading the file."
        );
    }

    return instance;
}

}