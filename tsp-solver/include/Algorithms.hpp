#pragma once
#include "Graph.hpp"
#include "TspInstance.hpp"


namespace tsp {
    class Solver {
        public:
            virtual ~Solver() = default;

            virtual Tour solve(const TspInstance& instance) const = 0;
    };

    // visits the cities in increasing order of their ids
    class TrivialSolver : public Solver {
        public:
            Tour solve(const TspInstance& instance) const override;
    };
}
