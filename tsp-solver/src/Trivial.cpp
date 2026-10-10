#include "Algorithms.hpp"
#include <algorithm>
#include <numeric>


namespace tsp {

Tour TrivialSolver::solve(const TspInstance& instance) const {
    Tour tour;

    // the cities are listed in increasing id order in every instance file
    for (std::size_t i = 0; i < instance.size(); i++) {
        tour.push_back(i);
    }

    return tour;
}

}
