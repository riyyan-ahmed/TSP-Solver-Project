#pragma once
#include <cstddef>
#include <vector>
#include "Graph.hpp"


namespace tsp {
    /** Minimum spanning tree of a complete graph, computed with Prim's algorithm and rooted at vertex 0. */
    class MinimumSpanningTree {
        std::vector<std::vector<std::size_t>> children;
        double totalWeight = 0.0;

        public:
            /** Vertex the tree grows from. */
            static constexpr std::size_t root = 0;

            /** Builds the tree in O(n^2), which suits complete graphs; ties go to the smallest vertex index. */
            explicit MinimumSpanningTree(const Graph& graph);

            /** Returns the number of vertices. */
            std::size_t size() const;

            /** Returns the sum of the edge weights, a lower bound on the length of any tour. */
            double weight() const;

            /** Returns the children of a vertex in increasing index order; throws std::out_of_range if invalid. */
            const std::vector<std::size_t>& childrenOf(std::size_t vertex) const;
    };
}
