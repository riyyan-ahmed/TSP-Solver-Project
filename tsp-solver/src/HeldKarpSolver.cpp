// Partly edited by Claude (Claude Opus 5.5), 2026-11-14: comment on the recurrence, include order, formatting
#include "HeldKarpSolver.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include "Subset.hpp"


namespace tsp {

Tour HeldKarpSolver::solve(const TspInstance& instance) const {
    const Graph& graph = instance.getGraph();
    const std::size_t n = graph.size();

    // with 3 cities or less every tour has the same length
    if (n <= 3) {
        Tour tour;
        for (std::size_t i = 0; i < n; i++) {
            tour.push_back(i);
        }
        return tour;
    }

    if (n > maxCities) {
        throw std::invalid_argument("Held-Karp is limited to " + std::to_string(maxCities) + " cities");
    }

    // weight() is called a lot so the distances are copied once into a matrix
    std::vector<std::vector<double>> d(n, std::vector<double>(n));
    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            d[i][j] = graph.weight(i, j);
        }
    }

    // city 0 is the start, element e of a subset is city e + 1
    const std::size_t m = n - 1;
    const double infinity = std::numeric_limits<double>::infinity();

    // cost[S * m + k] is the length of the shortest path that starts at city 0, visits every city of S once and ends at city k + 1 (k in S). It is filled by increasing size of S:
    //   cost[{k}][k] = d(0, k + 1)
    //   cost[S][k]   = min over i in S - {k} of cost[S - {k}][i] + d(i + 1, k + 1)
    // and the optimal tour length is the min over k of cost[all][k] + d(k + 1, 0)
    std::vector<double> cost((1ULL << m) * m, infinity);

    for (std::size_t k = 0; k < m; k++) {
        subset::Subset single(0, m);
        single.add(k);
        cost[single.index() * m + k] = d[0][k + 1];
    }

    for (std::size_t size = 2; size <= m; size++) {
        for (subset::SubsetIterator it(m, size); it != subset::SubsetIterator::end(); ++it) {
            const subset::Subset& s = *it;

            for (std::size_t k : s) {
                subset::Subset rest = s;
                rest.minus(k);

                double best = infinity;
                for (std::size_t i : rest) {
                    best = std::min(best, cost[rest.index() * m + i] + d[i + 1][k + 1]);
                }
                cost[s.index() * m + k] = best;
            }
        }
    }

    // pick the last city before going back to city 0
    const subset::Subset all(m, m);
    std::size_t last = 0;
    for (std::size_t k = 1; k < m; k++) {
        if (cost[all.index() * m + k] + d[k + 1][0] < cost[all.index() * m + last] + d[last + 1][0]) {
            last = k;
        }
    }

    // walk back through the table to find which city came before each one
    Tour tour;
    subset::Subset s = all;
    std::size_t k = last;
    while (true) {
        tour.push_back(k + 1);
        subset::Subset rest = s;
        rest.minus(k);
        if (rest.index() == 0) {
            break;
        }
        for (std::size_t i : rest) {
            // same sum as when the table was filled so == is exact here
            if (cost[rest.index() * m + i] + d[i + 1][k + 1] == cost[s.index() * m + k]) {
                k = i;
                break;
            }
        }
        s = rest;
    }
    tour.push_back(0);
    std::reverse(tour.begin(), tour.end());
    
    return tour;
}

}
