#pragma once
#include <utility>
#include <vector>
#include "Graph.hpp"



namespace tsp {
    /** Graph in form of points (x, y), used for NODE_COORD_SECTION instances. */
    class CoordinatesGraph : public Graph {
        std::vector<std::pair<double, double>> coordinates;

        public:
            /** Builds the graph from a non-empty list of coordinates. */
            explicit CoordinatesGraph(std::vector<std::pair<double, double>> coordinates);

            std::size_t size() const override;

            /** Returns the Euclidean distance between points i and j. */
            double weight(std::size_t i, std::size_t j) const override;
    };
}
