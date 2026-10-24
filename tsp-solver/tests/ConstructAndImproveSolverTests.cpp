#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <utility>
#include "ConstructAndImproveSolver.hpp"
#include "MatrixGraph.hpp"
#include "TwoOpt.hpp"

using namespace tsp;

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

    // Same four cities as in TwoOptTests.cpp: the best tour has length 13
    TspInstance fourCities() {
        return TspInstance("four", {1, 2, 3, 4}, std::make_unique<MatrixGraph>(DistanceMatrix{
            {0, 2, 1, 5},
            {2, 0, 3, 6},
            {1, 3, 0, 4},
            {5, 6, 4, 0}}));
    }
}

TEST(ConstructAndImproveSolver, RejectsAMissingConstructor) {
    EXPECT_THROW(ConstructAndImproveSolver(nullptr, std::make_unique<TwoOpt>()), std::invalid_argument);
}

TEST(ConstructAndImproveSolver, WithoutImproverReturnsTheConstructedTour) {
    ConstructAndImproveSolver solver(std::make_unique<FixedConstructor>(Tour{0, 2, 1, 3}));
    EXPECT_EQ(solver.solve(fourCities()), Tour({0, 2, 1, 3}));
}

TEST(ConstructAndImproveSolver, ImprovesTheConstructedTour) {
    ConstructAndImproveSolver solver(std::make_unique<FixedConstructor>(Tour{0, 2, 1, 3}),
                                     std::make_unique<TwoOpt>());
    TspInstance instance = fourCities();

    Tour tour = solver.solve(instance);

    EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(tour), 13.0);
}

TEST(ConstructAndImproveSolver, CanBeUsedThroughTheSolverInterface) {
    std::unique_ptr<Solver> solver = std::make_unique<ConstructAndImproveSolver>(
        std::make_unique<FixedConstructor>(Tour{0, 1, 2, 3}), std::make_unique<TwoOpt>());
    TspInstance instance = fourCities();

    EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(solver->solve(instance)), 13.0);
}
