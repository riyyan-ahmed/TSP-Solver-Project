// Partly edited by Claude (Claude Opus 5.5), 2026-11-13: helpers from TestHelpers.hpp, removed IsDeterministic and IsImprovedByTwoOptToTheOptimum (repeated in TwoOptTests.cpp)
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "CoordinatesGraph.hpp"
#include "MinimumSpanningTree.hpp"
#include "MstConstructor.hpp"
#include "TestHelpers.hpp"
#include "TwoOpt.hpp"

using namespace tsp;
using namespace helpers;

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

// ---------- All provided instances ----------

class MstOnProvidedInstances : public ::testing::TestWithParam<const char*> {};

TEST_P(MstOnProvidedInstances, BuildsAValidTourWithinTheBounds) {
    const std::string file = GetParam();
    TspInstance instance = load(file);
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
