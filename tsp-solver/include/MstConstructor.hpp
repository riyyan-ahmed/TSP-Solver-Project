#pragma once
#include "TourConstructor.hpp"


namespace tsp {
    /**
     * Visits the vertices in the depth-first order of a minimum spanning tree.
     * On metric graphs the tour is at most twice as long as the optimal tour.
     */
    class MstConstructor : public TourConstructor {
        public:
            Tour build(const Graph& graph) const override;
    };
}
