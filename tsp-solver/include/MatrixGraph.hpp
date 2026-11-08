#pragma once
#include <vector>
#include "Graph.hpp"


namespace tsp {
    /** distances[i][j] is the weight of the edge between vertices i and j. */
    using DistanceMatrix = std::vector<std::vector<double>>;

    /** Graph with already computed weights, used for LOWER_DIAG_ROW instances. */
    class MatrixGraph : public Graph {
        DistanceMatrix distances;

        public:
            /** Builds the graph from a non-empty square matrix. */
            explicit MatrixGraph(DistanceMatrix distances);

            std::size_t size() const override;

            double weight(std::size_t i, std::size_t j) const override;
    };
}
