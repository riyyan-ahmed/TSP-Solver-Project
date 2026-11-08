#pragma once
#include "Graph.hpp"
#include "TspInstance.hpp"


namespace tsp {
    /** Base class of every solving method. */
    class Solver {
        public:
            virtual ~Solver() = default;

            /** Returns a tour that visits every city of the instance once. */
            virtual Tour solve(const TspInstance& instance) const = 0;
    };

    /** Visits the cities in increasing order (Trivial method) of their ids (1 -> 2 -> ... -> n). */
    class TrivialSolver : public Solver {
        public:
            Tour solve(const TspInstance& instance) const override;
    };
}
