#pragma once
#include <cstddef>
#include <limits>
#include "TourImprover.hpp"


namespace tsp {
    /** 2-opt local search : replaces two edges by two shorter ones until no such move remains */
    class TwoOpt : public TourImprover {
        std::size_t maxPasses;

        public:
            /** Value of maxPasses meaning : repeating until no improving move remains */
            static constexpr std::size_t unlimited = std::numeric_limits<std::size_t>::max();

            /** maxPasses limits the number of full scans of the tour, to bound the time on large instances. */
            explicit TwoOpt(std::size_t maxPasses = unlimited);

            /** Applies every improving move as soon as the scan finds it */
            Tour improve(const Graph& graph, Tour tour) const override;
    };
}
