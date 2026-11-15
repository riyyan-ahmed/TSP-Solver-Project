#pragma once
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>
#include "MatrixGraph.hpp"
#include "TspParser.hpp"

#ifndef TSP_INSTANCES_DIR
#error "TSP_INSTANCES_DIR must be defined by the build system"
#endif


namespace helpers {
    /** Every instance file of the provided archive, relative to TSP-instances. */
    inline const char* const instanceFiles[] = {
        "coord/att48.tsp", "coord/berlin52.tsp", "coord/burma14.tsp", "coord/ch130.tsp",
        "coord/eil76.tsp", "coord/pr1002.tsp", "coord/pr299.tsp", "coord/pr76.tsp",
        "coord/st70.tsp", "coord/ulysses16.tsp",
        "lower_diag/dantzig42.tsp", "lower_diag/gr17.tsp", "lower_diag/gr21.tsp",
        "lower_diag/gr24.tsp", "lower_diag/gr48.tsp"};

    /** Parses a provided instance, the path being relative to TSP-instances. */
    inline tsp::TspInstance load(const std::string& relativePath) {
        tsp::TspParser parser;
        return parser.parse(std::string(TSP_INSTANCES_DIR) + "/" + relativePath);
    }

    /** True if the tour visits every vertex from 0 to n - 1 exactly once. */
    inline bool isPermutation(tsp::Tour tour, std::size_t n) {
        tsp::Tour expected(n);
        std::iota(expected.begin(), expected.end(), 0);
        std::sort(tour.begin(), tour.end());
        return tour == expected;
    }

    /** Cities A, B, C, D = 0, 1, 2, 3: the three possible tours have length 14, 13 and 15. */
    inline tsp::MatrixGraph fourCities() {
        return tsp::MatrixGraph({{0, 2, 1, 5},
                                 {2, 0, 3, 6},
                                 {1, 3, 0, 4},
                                 {5, 6, 4, 0}});
    }
}
