#pragma once
#include "Graph.hpp"


namespace tsp {
    /** Improves an existing tour of a graph (local search, e.g. 2-opt). */
    class TourImprover {
        public:
            virtual ~TourImprover() = default;

            /** Returns a tour of the same vertices that is never longer than the given one. */
            virtual Tour improve(const Graph& graph, Tour tour) const = 0;
    };
}
