#pragma once
#include <utility>
#include <vector>
#include "Graph.hpp"

namespace tsp
{
    // How the distance between two cities is computed (EDGE_WEIGHT_TYPE of the file)
    enum class EdgeWeightType
    {
        Euclidean, // Also used as a base-case when the file gives no type
        Euc2d,
        Att,
        Geo
    };
    /** Graph in form of points (x, y), used for NODE_COORD_SECTION instances. */
    class CoordinatesGraph : public Graph
    {
        std::vector<std::pair<double, double>> coordinates;
        EdgeWeightType weightType;

    public:
        /** Builds the graph from a non-empty list of coordinates. */
        explicit CoordinatesGraph(std::vector<std::pair<double, double>> coordinates,
                                  EdgeWeightType weightType = EdgeWeightType::Euclidean);

        std::size_t size() const override;

        /** Returns the Euclidean distance between points i and j. */
        double weight(std::size_t i, std::size_t j) const override;
    };
}
