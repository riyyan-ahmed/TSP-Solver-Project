#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include "Algorithms.hpp"
#include "TspParser.hpp"

using namespace tsp;

#ifndef TSP_INSTANCES_DIR
#error "TSP_INSTANCES_DIR must be defined by the build system"
#endif

namespace {
    const char* const instanceFiles[] = {
        "coord/att48.tsp", "coord/berlin52.tsp", "coord/burma14.tsp", "coord/ch130.tsp",
        "coord/eil76.tsp", "coord/pr1002.tsp", "coord/pr299.tsp", "coord/pr76.tsp",
        "coord/st70.tsp", "coord/ulysses16.tsp",
        "lower_diag/dantzig42.tsp", "lower_diag/gr17.tsp", "lower_diag/gr21.tsp",
        "lower_diag/gr24.tsp", "lower_diag/gr48.tsp"};

    TspInstance load(const std::string& relativePath) {
        TspParser parser;
        return parser.parse(std::string(TSP_INSTANCES_DIR) + "/" + relativePath);
    }

    bool isPermutation(const Tour& tour, std::size_t n) {
        Tour sorted = tour;
        std::sort(sorted.begin(), sorted.end());
        Tour expected(n);
        std::iota(expected.begin(), expected.end(), 0);
        return sorted == expected;
    }
}

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

TEST(ProvidedInstanceValues, Gr17MatrixIsSymmetric) {
    TspInstance instance = load("lower_diag/gr17.tsp");
    const Graph& g = instance.getGraph();
    for (std::size_t i = 0; i < g.size(); i++)
        for (std::size_t j = 0; j < g.size(); j++)
            EXPECT_DOUBLE_EQ(g.weight(i, j), g.weight(j, i));
}

TEST(TrivialSolver, ReturnsIdentityPermutation) {
    TspInstance instance = load("coord/berlin52.tsp");
    Tour tour = TrivialSolver().solve(instance);
    ASSERT_EQ(tour.size(), 52u);
    for (std::size_t i = 0; i < tour.size(); i++) {
        EXPECT_EQ(tour[i], i);
    }
}

TEST(TrivialSolver, HandlesSingleCity) {
    TspParser parser;
    std::istringstream input("NAME: one\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 5 5\nEOF\n");
    TspInstance instance = parser.parse(input);
    EXPECT_EQ(TrivialSolver().solve(instance), Tour({0}));
}
