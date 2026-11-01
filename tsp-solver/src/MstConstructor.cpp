#include "MstConstructor.hpp"
#include "MinimumSpanningTree.hpp"
#include <vector>


namespace tsp {

Tour MstConstructor::build(const Graph& graph) const {
    if (graph.size() == 0) {
        return {};
    }

    const MinimumSpanningTree tree(graph);

    // Depth-first walk with an explicit stack instead of recursion, so a tree shaped like a
    // long path (thousands of levels) cannot overflow the call stack. A tree has no cycles,
    // so every vertex is reached exactly once and no "visited" list is needed.
    Tour tour;
    tour.reserve(graph.size());
    std::vector<std::size_t> toVisit{MinimumSpanningTree::root};

    while (!toVisit.empty()) {
        const std::size_t vertex = toVisit.back();
        toVisit.pop_back();
        tour.push_back(vertex);

        // pushed in reverse, so the child with the smallest index is visited first
        const auto& children = tree.childrenOf(vertex);
        toVisit.insert(toVisit.end(), children.rbegin(), children.rend());
    }

    return tour;
}

}
