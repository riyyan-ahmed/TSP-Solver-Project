#include "TwoOpt.hpp"
#include <algorithm>
#include <stdexcept>


namespace {
    // smaller changes are treated as rounding noise, so equal tours never swap forever
    constexpr double epsilon = 1e-9;

    /**
     * Change of tour length when the edges (t[i], t[i+1]) and (t[j], t[j+1]) are replaced
     * by (t[i], t[j]) and (t[i+1], t[j+1]). A negative value means a shorter tour.
     */
    double gain(const tsp::Graph& graph, const tsp::Tour& tour, std::size_t i, std::size_t j) {
        const std::size_t a = tour[i];
        const std::size_t b = tour[i + 1];
        const std::size_t c = tour[j];
        const std::size_t d = tour[(j + 1) % tour.size()];

        return graph.weight(a, c) + graph.weight(b, d) - graph.weight(a, b) - graph.weight(c, d);
    }

    /** Applies the move evaluated by gain(): reversing t[i+1..j] reconnects the tour with the new edges. */
    void applyMove(tsp::Tour& tour, std::size_t i, std::size_t j) {
        std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
    }
}


namespace tsp {

TwoOpt::TwoOpt(std::size_t maxPasses) : maxPasses(maxPasses) {}

Tour TwoOpt::improve(const Graph& graph, Tour tour) const {
    if (tour.size() != graph.size()) {
        throw std::invalid_argument("The tour must contain every vertex of the graph");
    }

    const std::size_t n = tour.size();

    // with fewer than 4 vertices there are no two edges without a common vertex
    if (n < 4) {
        return tour;
    }

    bool improved = true;

    for (std::size_t pass = 0; improved && pass < maxPasses; pass++) {
        improved = false;

        for (std::size_t i = 0; i + 2 < n; i++) {
            for (std::size_t j = i + 2; j < n; j++) {
                // the edges (t[0], t[1]) and (t[n-1], t[0]) share t[0]
                if (i == 0 && j == n - 1) {
                    continue;
                }

                if (gain(graph, tour, i, j) < -epsilon) {
                    applyMove(tour, i, j);
                    improved = true;
                }
            }
        }
    }

    return tour;
}

}
