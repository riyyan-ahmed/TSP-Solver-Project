#include "MatrixGraph.hpp"
#include <stdexcept>
#include <utility>

namespace tsp {

MatrixGraph::MatrixGraph(DistanceMatrix distances) : distances(std::move(distances)) {
    // Generated with Claude Code (Claude Opus 5.5), 2026-10-07
    for (const auto& row : this->distances) {
        if (row.size() != this->distances.size()) {
            throw std::invalid_argument("Distance matrix must be square.");
        }
    }
}

std::size_t MatrixGraph::size() const {
    return distances.size();
}

double MatrixGraph::weight(std::size_t i, std::size_t j) const {
    return distances[i][j];
}

}
