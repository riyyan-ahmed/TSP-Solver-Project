#pragma once
#include "TourConstructor.hpp"


namespace tsp {
    /**
     * Visit the vertices in the depth-first order of minimum spanning tree. As in metric graphs the tour is at most twice as long as the optimal tour.
     */
    class MstConstructor : public TourConstructor {
        public:
            Tour build(const Graph& graph) const override;
    };
}
