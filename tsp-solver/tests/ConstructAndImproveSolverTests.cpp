// Partly edited by Claude (Claude Opus 5.5), 2026-11-15: four cities from TestHelpers.hpp, removed CanBeUsedThroughTheSolverInterface (the same as ImprovesTheConstructedTour)
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <utility>
#include "ConstructAndImproveSolver.hpp"
#include "TestHelpers.hpp"
#include "TwoOpt.hpp"

using namespace tsp;
using namespace helpers;

namespace {
    // Test double: always builds the same tour, so the solver can be tested before
    // the real constructors (minimum spanning tree, nearest neighbour) exist
    class FixedConstructor : public TourConstructor {
        Tour tour;

        public:
            explicit FixedConstructor(Tour tour) : tour(std::move(tour)) {}

            Tour build(const Graph&) const override {
                return tour;
            }
    };

    // the best tour of the four cities has length 13
    TspInstance fourCityInstance() {
        return TspInstance("four", {1, 2, 3, 4}, std::make_unique<MatrixGraph>(fourCities()));
    }
}

TEST(ConstructAndImproveSolver, RejectsAMissingConstructor) {
    EXPECT_THROW(ConstructAndImproveSolver(nullptr, std::make_unique<TwoOpt>()), std::invalid_argument);
}

TEST(ConstructAndImproveSolver, WithoutImproverReturnsTheConstructedTour) {
    ConstructAndImproveSolver solver(std::make_unique<FixedConstructor>(Tour{0, 2, 1, 3}));
    EXPECT_EQ(solver.solve(fourCityInstance()), Tour({0, 2, 1, 3}));
}

TEST(ConstructAndImproveSolver, ImprovesTheConstructedTour) {
    ConstructAndImproveSolver solver(std::make_unique<FixedConstructor>(Tour{0, 2, 1, 3}),
                                     std::make_unique<TwoOpt>());
    TspInstance instance = fourCityInstance();

    Tour tour = solver.solve(instance);

    EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(tour), 13.0);
}
