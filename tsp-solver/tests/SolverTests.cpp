#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include <sstream>
#include "Algorithms.hpp"
#include "ConstructAndImproveSolver.hpp"
#include "NearestNeighbourConstructor.hpp"
#include "TestHelpers.hpp"
#include "TwoOpt.hpp"

using namespace tsp;
using namespace helpers;

class ProvidedInstances : public ::testing::TestWithParam<const char*> {};

TEST_P(ProvidedInstances, ParseAndTrivialSolve) {
    TspInstance instance = load(GetParam());
    ASSERT_GT(instance.size(), 0u);

    for (std::size_t i = 0; i < instance.size(); i++) {
        EXPECT_EQ(instance.getCityId(i), static_cast<int>(i) + 1);
        EXPECT_DOUBLE_EQ(instance.getGraph().weight(i, i), 0.0);
    }

    Tour tour = TrivialSolver().solve(instance);
    EXPECT_TRUE(isPermutation(tour, instance.size()));
    EXPECT_GT(instance.getGraph().tourLength(tour), 0.0);
}

INSTANTIATE_TEST_SUITE_P(AllFiles, ProvidedInstances, ::testing::ValuesIn(instanceFiles));

TEST(ProvidedInstanceSizes, MatchTheirNames) {
    EXPECT_EQ(load("coord/berlin52.tsp").size(), 52u);
    EXPECT_EQ(load("coord/pr1002.tsp").size(), 1002u);
    EXPECT_EQ(load("lower_diag/gr17.tsp").size(), 17u);
    EXPECT_EQ(load("lower_diag/dantzig42.tsp").size(), 42u);
}

TEST(TrivialSolver, ReturnsTheExampleTourOfTheSubject) {
    // section 2.4.1 of the subject: the tour must be 1 -> 2 -> 3 -> 4 -> 5
    TspParser parser;
    std::istringstream input("NAME: example.tsp\nTYPE: TSP\nDIMENSION: 5\nNODE_COORD_SECTION\n"
                             "1 565.0 575.0\n2 25.0 185.0\n3 345.0 750.0\n4 945.0 685.0\n5 845.0 655.0\nEOF\n");
    EXPECT_EQ(TrivialSolver().solve(parser.parse(input)), Tour({0, 1, 2, 3, 4}));
}

TEST(TrivialSolver, HandlesSingleCity) {
    TspParser parser;
    std::istringstream input("NAME: one\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 5 5\nEOF\n");
    TspInstance instance = parser.parse(input);
    EXPECT_EQ(TrivialSolver().solve(instance), Tour({0}));
}

TEST(ProvidedInstanceValues, Ulysses16OptimalTourHasLength6859) {
    // optimal tour from TSPLIB, cities numbered from 1
    const Tour optimal = {1, 14, 13, 12, 7, 6, 15, 5, 11, 9, 10, 16, 3, 2, 4, 8};
    Tour tour;
    for (std::size_t city : optimal) {
        tour.push_back(city - 1);
    }
    EXPECT_DOUBLE_EQ(load("coord/ulysses16.tsp").getGraph().tourLength(tour), 6859.0);
}

TEST(ProvidedInstanceValues, TourLengthsAreWholeNumbersAndNotBelowTheOptimum) {
    // optimal tour lengths from TSPLIB
    struct Known {
        const char* file;
        double optimum;
    };
    const Known known[] = {
        {"coord/att48.tsp", 10628}, {"coord/berlin52.tsp", 7542}, {"coord/burma14.tsp", 3323},
        {"coord/ch130.tsp", 6110}, {"coord/eil76.tsp", 538}, {"coord/pr1002.tsp", 259045},
        {"coord/pr299.tsp", 48191}, {"coord/pr76.tsp", 108159}, {"coord/st70.tsp", 675},
        {"coord/ulysses16.tsp", 6859}, {"lower_diag/dantzig42.tsp", 699}, {"lower_diag/gr17.tsp", 2085},
        {"lower_diag/gr21.tsp", 2707}, {"lower_diag/gr24.tsp", 1272}, {"lower_diag/gr48.tsp", 5046}};

    ConstructAndImproveSolver solver(std::make_unique<NearestNeighbourConstructor>(), std::make_unique<TwoOpt>());

    for (const Known& k : known) {
        SCOPED_TRACE(k.file);
        TspInstance instance = load(k.file);
        const double length = instance.getGraph().tourLength(solver.solve(instance));
        EXPECT_DOUBLE_EQ(length, std::floor(length));
        EXPECT_GE(length, k.optimum);
    }
}
