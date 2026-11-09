#include "CoordinatesGraph.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace tsp
{

    CoordinatesGraph::CoordinatesGraph(std::vector<std::pair<double, double>> coordinates,
                                       EdgeWeightType weightType)
        : coordinates(std::move(coordinates)), weightType(weightType)
    {

        if (this->coordinates.empty())
        {
            throw std::invalid_argument("Coordinates graph cannot be empty");
        }
    }

    std::size_t CoordinatesGraph::size() const
    {
        return coordinates.size();
    }

    double CoordinatesGraph::weight(std::size_t i, std::size_t j) const
    {
        if (i >= coordinates.size() || j >= coordinates.size())
        {
            throw std::out_of_range("City index out of range");
        }

        const double dx = coordinates[i].first - coordinates[j].first;
        const double dy = coordinates[i].second - coordinates[j].second;

        const double distance = std::sqrt(dx * dx + dy * dy);

        // each EDGE_WEIGHT_TYPE has its own formula but all use the plain distance for now

        switch (weightType)
        {
        case EdgeWeightType::Euc2d:
            return distance;
        case EdgeWeightType::Att:
            return distance;
        case EdgeWeightType::Geo:
            return distance;
        case EdgeWeightType::Euclidean:
            return distance;
        }
        return distance;
    }

}
