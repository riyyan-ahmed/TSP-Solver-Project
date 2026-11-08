#include "Algorithms.hpp"


namespace tsp {

Tour TrivialSolver::solve(const TspInstance& instance) const {
    Tour tour;

    // the parsers store the cities in increasing id order (1, 2, ..., n)
    for (std::size_t i = 0; i < instance.size(); i++) {
        tour.push_back(i);
    }

    return tour;
}

}
