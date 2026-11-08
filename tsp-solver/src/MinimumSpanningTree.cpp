#include "MinimumSpanningTree.hpp"
#include <algorithm>
#include <limits>


namespace tsp {

MinimumSpanningTree::MinimumSpanningTree(const Graph& graph) : children(graph.size()) {
    const std::size_t n = graph.size();

    // for each vertex outside the tree: its cheapest edge to the tree, and the tree vertex at the other end
    std::vector<double> cheapest(n, std::numeric_limits<double>::infinity());
    std::vector<std::size_t> parent(n, root);
    std::vector<bool> inTree(n, false);

    if (n > 0) {
        cheapest[root] = 0.0;
    }

    for (std::size_t added = 0; added < n; added++) {
        // the closest vertex outside the tree joins it (strict < keeps the smallest index on ties)
        std::size_t next = n;
        for (std::size_t v = 0; v < n; v++) {
            if (!inTree[v] && (next == n || cheapest[v] < cheapest[next])) {
                next = v;
            }
        }

        inTree[next] = true;
        totalWeight += cheapest[next];
        if (next != root) {
            children[parent[next]].push_back(next);
        }

        // the new tree vertex may offer a cheaper connection to the vertices still outside
        for (std::size_t v = 0; v < n; v++) {
            if (inTree[v]) {
                continue;
            }
            const double w = graph.weight(next, v);
            if (w < cheapest[v]) {
                cheapest[v] = w;
                parent[v] = next;
            }
        }
    }

    for (auto& list : children) {
        std::sort(list.begin(), list.end());
    }
}

std::size_t MinimumSpanningTree::size() const {
    return children.size();
}

double MinimumSpanningTree::weight() const {
    return totalWeight;
}

const std::vector<std::size_t>& MinimumSpanningTree::childrenOf(std::size_t vertex) const {
    return children.at(vertex);
}

}
