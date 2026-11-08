#pragma once
#include "Graph.hpp"


namespace tsp {
    /** Builds a first tour of a graph (e.g. from a minimum spanning tree or by nearest neighbour). */
    class TourConstructor {
        public:
            virtual ~TourConstructor() = default;

            /** Returns a tour that visits every vertex of the graph exactly once. */
            virtual Tour build(const Graph& graph) const = 0;
    };
}
