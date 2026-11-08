#include "NearestNeighbourConstructor.hpp"
#include <vector>


namespace {
    /** Returns the unvisited vertex closest to current; strict < keeps the smallest index on ties. */
    std::size_t nearestUnvisited(const tsp::Graph& graph, std::size_t current, const std::vector<bool>& visited) {
        const std::size_t n = graph.size();
        std::size_t nearest = n;
        double nearestDistance = 0.0;

        for (std::size_t v = 0; v < n; v++) {
            if (visited[v]) {
                continue;
            }

            const double distance = graph.weight(current, v);
            if (nearest == n || distance < nearestDistance) {
                nearest = v;
                nearestDistance = distance;
            }
        }

        return nearest;
    }
}


namespace tsp {

Tour NearestNeighbourConstructor::build(const Graph& graph) const {
    const std::size_t n = graph.size();

    if (n == 0) {
        return {};
    }

    Tour tour;
    tour.reserve(n);
    std::vector<bool> visited(n, false);

    std::size_t current = start;
    visited[current] = true;
    tour.push_back(current);

    while (tour.size() < n) {
        current = nearestUnvisited(graph, current, visited);
        visited[current] = true;
        tour.push_back(current);
    }

    return tour;
}

}
