#pragma once
#include <utility>
#include <vector>
#include "Graph.hpp"

namespace tsp {
    // Graph built from city coordinates
    class CoordinatesGraph : public Graph {
        std::vector<std::pair<double, double>> coordinates;

    public:
        explicit CoordinatesGraph(std::vector<std::pair<double, double>> coordinates);

        std::size_t size() const override;

        double weight(std::size_t i, std::size_t j) const override;
    };
}