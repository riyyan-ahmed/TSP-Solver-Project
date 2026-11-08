#pragma once
#include <vector>


namespace tsp {
    /** Order in which the vertices are visited (0 based indices). */
    using Tour = std::vector<std::size_t>;

    /** Complete, undirected and weighted graph (data structure choice) used by the solvers. */
    class Graph {
        public:
            virtual ~Graph() = default;

            /** Returns the number of vertices (nodes). */
            virtual std::size_t size() const = 0;

            /** Returns the weight of the any edge between vertices i and j. */
            virtual double weight(std::size_t i, std::size_t j) const = 0;

            /** Returns the length of the tour, including the way back to the start. */
            double tourLength(const Tour& tour) const;
    };
}
