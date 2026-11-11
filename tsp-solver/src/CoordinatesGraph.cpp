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
        // GEO coordinates are written as DDD.MM (degrees and minutes) converted once to radians
        if (this->weightType == EdgeWeightType::Geo)
        {
            const double pi = 3.141592;

            for (const auto &point : this->coordinates)
            {
                // the degrees are truncated, not rounded
                const int latDegrees = static_cast<int>(point.first);
                const double latMinutes = point.first - latDegrees;
                const int lonDegrees = static_cast<int>(point.second);
                const double lonMinutes = point.second - lonDegrees;

                radians.push_back({pi * (latDegrees + 5.0 * latMinutes / 3.0) / 180.0,
                                   pi * (lonDegrees + 5.0 * lonMinutes / 3.0) / 180.0});
            }
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

        // each EDGE_WEIGHT_TYPE has its own TSPLIB formula

        switch (weightType)
        {
        case EdgeWeightType::Euc2d:
            return static_cast<int>(distance + 0.5); // TSPLIB nint, rounds to the nearest integer
        case EdgeWeightType::Att:
        {
            // ATT is a pseudo Euclidean distance its rounded up when nint rounded down
            const double r = std::sqrt((dx * dx + dy * dy) / 10.0);
            const int t = static_cast<int>(r + 0.5);
            return (t < r) ? t + 1 : t;
        }
        case EdgeWeightType::Geo:
        {
            // TSPLIB gives the same city a distance of 1 with this formula so we want 0
            if (i == j)
            {
                return 0.0;
            }

            const double earthRadius = 6378.388;
            const double q1 = std::cos(radians[i].second - radians[j].second);
            const double q2 = std::cos(radians[i].first - radians[j].first);
            const double q3 = std::cos(radians[i].first + radians[j].first);

            double a = 0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3);
            if (a > 1.0)
            {
                a = 1.0; // two cities at the same spot can give 1.0000000000000002 and acos of that is NaN
            }

            return static_cast<int>(earthRadius * std::acos(a) + 1.0);
        }

        case EdgeWeightType::Euclidean:
            return distance;
        }
        return distance;
    }

}
