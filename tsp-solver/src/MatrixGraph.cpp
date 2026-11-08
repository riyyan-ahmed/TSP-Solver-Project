#include <stdexcept>
#include <utility>
#include "MatrixGraph.hpp"



namespace tsp {

MatrixGraph::MatrixGraph(DistanceMatrix distances) : distances(std::move(distances)) {
    // Generated with Claude (Claude Opus 5.5), 2026-10-07
    if (this->distances.empty()) {
        throw std::invalid_argument("Distance matrix cannot be empty");
    }

    for (const auto& row : this->distances) {
        if (row.size() != this->distances.size()) {
            throw std::invalid_argument("Distance matrix must be square");
        }
    }
}

std::size_t MatrixGraph::size() const {
    return distances.size();
}

double MatrixGraph::weight(std::size_t i, std::size_t j) const {
    if (i >= distances.size() || j >= distances.size()) {
        throw std::out_of_range("City index is out of range");
    }

    return distances[i][j];
}

}
