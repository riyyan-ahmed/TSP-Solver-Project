#pragma once
#include <cstddef>
#include "Algorithms.hpp"

namespace tsp {
    // dynamic programming over the subsets of cities
    class HeldKarpSolver : public Solver {
        public:
            // the table has (n - 1) * 2^(n - 1) entries, 24 cities already need about 1.5 GB
            static constexpr std::size_t maxCities = 24;
            Tour solve(const TspInstance& instance) const override;
    };
}
