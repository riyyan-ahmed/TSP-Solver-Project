#pragma once
#include "Algorithms.hpp"

namespace tsp {
    // dynamic programming over the subsets of cities
    class HeldKarpSolver : public Solver {
        public:
            Tour solve(const TspInstance& instance) const override;
    };
}
