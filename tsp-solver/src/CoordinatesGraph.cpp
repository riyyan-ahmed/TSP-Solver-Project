#include "CoordinatesGraph.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>


namespace tsp {

CoordinatesGraph::CoordinatesGraph(std::vector<std::pair<double, double>> coordinates)
    : coordinates(std::move(coordinates)) {
    if (this->coordinates.empty()) {
        throw std::invalid_argument("Coordinates graph cannot be empty");
    }
}

std::size_t CoordinatesGraph::size() const {
    return coordinates.size();
}

double CoordinatesGraph::weight(std::size_t i, std::size_t j) const {
    if (i >= coordinates.size() || j >= coordinates.size()) {
        throw std::out_of_range("City index out of range");
    }

    const double dx = coordinates[i].first - coordinates[j].first;
    const double dy = coordinates[i].second - coordinates[j].second;

    return std::sqrt(dx * dx + dy * dy);
}

}
