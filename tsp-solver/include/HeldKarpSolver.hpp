#pragma once
#include <cstddef>
#include "Algorithms.hpp"


namespace tsp {
    /**
     * Exact method (Held-Karp): dynamic programming over the subsets of cities. Its table has (n - 1) 2^(n - 1) entries of 8 bytes, so it only works on small instances
     */
    class HeldKarpSolver : public Solver {
        public:
            /** Largest instance accepted only: 24 cities already need almost about 1.5 GB of memory. */
            static constexpr std::size_t maxCities = 24;

            /** Returns an optimal tour; throws std::invalid_argument above maxCities */
            Tour solve(const TspInstance& instance) const override;
    };
}
