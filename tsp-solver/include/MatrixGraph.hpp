#pragma once
#include <vector>
#include "Graph.hpp"


namespace tsp {
    // A 2D vector where the element at row i and column j contains weight / distance between i and j vertices.
    using DistanceMatrix = std::vector<std::vector<double>>;

    // used for our LOWER_DIAG instances
    class MatrixGraph : public Graph {
        DistanceMatrix distances;

        public:
            explicit MatrixGraph(DistanceMatrix distances);

            std::size_t size() const override;

            double weight(std::size_t i, std::size_t j) const override;
    };
}
