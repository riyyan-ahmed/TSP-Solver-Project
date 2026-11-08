#pragma once
#include <memory>
#include "Algorithms.hpp"
#include "TourConstructor.hpp"
#include "TourImprover.hpp"


namespace tsp {
    /** Solver in two stages: a constructor builds a first tour, then an optional improver shortens it. */
    class ConstructAndImproveSolver : public Solver {
        std::unique_ptr<TourConstructor> constructor;
        std::unique_ptr<TourImprover> improver;

        public:
            /** Takes ownership of both stages; the constructor is required, the improver may be null. */
            explicit ConstructAndImproveSolver(std::unique_ptr<TourConstructor> constructor,
                                               std::unique_ptr<TourImprover> improver = nullptr);

            Tour solve(const TspInstance& instance) const override;
    };
}
