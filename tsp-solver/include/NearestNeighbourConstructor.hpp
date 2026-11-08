#pragma once
#include <cstddef>
#include "TourConstructor.hpp"


namespace tsp {
    /**
     * Starts at a fixed vertex and always moves to the closest vertex not visited yet, in O(n^2).
     * Unlike MstConstructor, it gives no guarantee on the length of the tour.
     */
    class NearestNeighbourConstructor : public TourConstructor {
        public:
            /** Vertex every tour starts from. */
            static constexpr std::size_t start = 0;

            Tour build(const Graph& graph) const override;
    };
}
