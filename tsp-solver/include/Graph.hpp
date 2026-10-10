#pragma once
#include <cstddef>
#include <vector>


namespace tsp {
    // Order in which vertices are visited, as 0-based vertex indices
    using Tour = std::vector<std::size_t>;

    // Our graphs for TSP is complete, undirected and weighted
    class Graph {
        public:
            virtual ~Graph() = default;

            virtual std::size_t size() const = 0;

            virtual double weight(std::size_t i, std::size_t j) const = 0;

            double tourLength(const Tour& tour) const;
    };
}
