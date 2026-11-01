#include <gtest/gtest.h>
#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include "ConstructAndImproveSolver.hpp"
#include "CoordinatesGraph.hpp"
#include "MatrixGraph.hpp"
#include "MinimumSpanningTree.hpp"
#include "MstConstructor.hpp"
#include "TspParser.hpp"
#include "TwoOpt.hpp"

#ifndef TSP_INSTANCES_DIR
#error "TSP_INSTANCES_DIR must be defined by the build system"
#endif

using namespace tsp;

namespace {
    // Cities A, B, C, D = 0, 1, 2, 3 (same example as TwoOptTests.cpp): the best tour has length 13
    MatrixGraph fourCities() {
        return MatrixGraph({{0, 2, 1, 5},
                            {2, 0, 3, 6},
                            {1, 3, 0, 4},
                            {5, 6, 4, 0}});
    }

    bool isPermutation(Tour tour, std::size_t n) {
        Tour expected(n);
        std::iota(expected.begin(), expected.end(), 0);
        std::sort(tour.begin(), tour.end());
        return tour == expected;
    }
}

// ---------- MinimumSpanningTree ----------

TEST(MinimumSpanningTree, FindsTheTreeOfTheFourCityExample) {
    MinimumSpanningTree tree(fourCities());   // edges A-C (1), A-B (2), C-D (4)

    EXPECT_EQ(tree.size(), 4u);
    EXPECT_DOUBLE_EQ(tree.weight(), 7.0);
    EXPECT_EQ(tree.childrenOf(0), std::vector<std::size_t>({1, 2}));
    EXPECT_TRUE(tree.childrenOf(1).empty());
    EXPECT_EQ(tree.childrenOf(2), std::vector<std::size_t>({3}));
    EXPECT_TRUE(tree.childrenOf(3).empty());
}

TEST(MinimumSpanningTree, FindsTheTreeOfTheSpecificationExample) {
    // figure 2 of the subject: the tree is the path A-B-C-D (5 + 4 + 3)
    MinimumSpanningTree tree(MatrixGraph({{0, 5, 8, 6}, {5, 0, 4, 7}, {8, 4, 0, 3}, {6, 7, 3, 0}}));
    EXPECT_DOUBLE_EQ(tree.weight(), 12.0);
}

TEST(MinimumSpanningTree, ListsChildrenInIncreasingIndexOrder) {
    // every city is closest to city 0, but they join the tree in the order 2, 3, 4, 1
    CoordinatesGraph star({{0, 0}, {-4, 0}, {0, 1}, {2, 0}, {0, -3}});
    MinimumSpanningTree tree(star);

    EXPECT_EQ(tree.childrenOf(0), std::vector<std::size_t>({1, 2, 3, 4}));
    EXPECT_DOUBLE_EQ(tree.weight(), 1.0 + 2.0 + 3.0 + 4.0);
}

TEST(MinimumSpanningTree, HandlesOneAndTwoCities) {
    MinimumSpanningTree one(MatrixGraph({{0}}));
    EXPECT_DOUBLE_EQ(one.weight(), 0.0);
    EXPECT_TRUE(one.childrenOf(0).empty());

    MinimumSpanningTree two(MatrixGraph({{0, 7}, {7, 0}}));
    EXPECT_DOUBLE_EQ(two.weight(), 7.0);
    EXPECT_EQ(two.childrenOf(0), std::vector<std::size_t>({1}));
}

TEST(MinimumSpanningTree, RejectsAnInvalidVertex) {
    MinimumSpanningTree tree(fourCities());
    EXPECT_THROW(tree.childrenOf(4), std::out_of_range);
}

// ---------- MstConstructor ----------

TEST(MstConstructor, BuildsTheDepthFirstTourOfTheFourCityExample) {
    MatrixGraph graph = fourCities();
    Tour tour = MstConstructor().build(graph);

    EXPECT_EQ(tour, Tour({0, 1, 2, 3}));                 // A B C D
    EXPECT_DOUBLE_EQ(graph.tourLength(tour), 14.0);      // at most 2 x 7
}

TEST(MstConstructor, FollowsADeepTreeToTheEnd) {
    // cities on a line, numbered out of order: the tree is the path 0-2-3-1
    CoordinatesGraph line({{0, 0}, {3, 0}, {1, 0}, {2, 0}});
    Tour tour = MstConstructor().build(line);

    EXPECT_EQ(tour, Tour({0, 2, 3, 1}));
    EXPECT_DOUBLE_EQ(line.tourLength(tour), 6.0);        // exactly 2 x the tree weight 3
}

TEST(MstConstructor, HandlesOneAndTwoCities) {
    EXPECT_EQ(MstConstructor().build(MatrixGraph({{0}})), Tour({0}));
    EXPECT_EQ(MstConstructor().build(MatrixGraph({{0, 7}, {7, 0}})), Tour({0, 1}));
}

TEST(MstConstructor, IsDeterministic) {
    MatrixGraph graph = fourCities();
    EXPECT_EQ(MstConstructor().build(graph), MstConstructor().build(graph));
}

TEST(MstConstructor, IsImprovedByTwoOptToTheOptimum) {
    TspInstance instance("four", {1, 2, 3, 4}, std::make_unique<MatrixGraph>(fourCities()));
    ConstructAndImproveSolver solver(std::make_unique<MstConstructor>(), std::make_unique<TwoOpt>());

    EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(solver.solve(instance)), 13.0);
}

// ---------- All provided instances ----------

namespace {
    const char* const instanceFiles[] = {
        "coord/att48.tsp", "coord/berlin52.tsp", "coord/burma14.tsp", "coord/ch130.tsp",
        "coord/eil76.tsp", "coord/pr1002.tsp", "coord/pr299.tsp", "coord/pr76.tsp",
        "coord/st70.tsp", "coord/ulysses16.tsp",
        "lower_diag/dantzig42.tsp", "lower_diag/gr17.tsp", "lower_diag/gr21.tsp",
        "lower_diag/gr24.tsp", "lower_diag/gr48.tsp"};
}

class MstOnProvidedInstances : public ::testing::TestWithParam<const char*> {};

TEST_P(MstOnProvidedInstances, BuildsAValidTourWithinTheBounds) {
    const std::string file = GetParam();
    TspParser parser;
    TspInstance instance = parser.parse(std::string(TSP_INSTANCES_DIR) + "/" + file);
    const Graph& graph = instance.getGraph();

    const MinimumSpanningTree tree(graph);
    const Tour tour = MstConstructor().build(graph);
    const Tour improved = TwoOpt().improve(graph, tour);

    // a tree on n vertices has n - 1 edges
    std::size_t edges = 0;
    for (std::size_t v = 0; v < tree.size(); v++) {
        edges += tree.childrenOf(v).size();
    }
    EXPECT_EQ(edges, graph.size() - 1);

    EXPECT_TRUE(isPermutation(tour, graph.size()));
    EXPECT_EQ(tour.front(), MinimumSpanningTree::root);

    // removing one edge of any tour gives a spanning tree, so the tree weight is a lower bound
    EXPECT_LE(tree.weight(), graph.tourLength(improved));
    EXPECT_LE(graph.tourLength(improved), graph.tourLength(tour));

    // the factor 2 guarantee needs the triangle inequality: it holds for the coordinate files,
    // but the provided matrix files (gr*, dantzig42) do not satisfy it
    if (file.rfind("coord/", 0) == 0) {
        EXPECT_LE(graph.tourLength(tour), 2.0 * tree.weight() + 1e-9);
    }
}

INSTANTIATE_TEST_SUITE_P(AllFiles, MstOnProvidedInstances, ::testing::ValuesIn(instanceFiles));
