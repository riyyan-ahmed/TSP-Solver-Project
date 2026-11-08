#include "ConstructAndImproveSolver.hpp"
#include <stdexcept>
#include <utility>


namespace tsp {

ConstructAndImproveSolver::ConstructAndImproveSolver(std::unique_ptr<TourConstructor> constructor,
                                                     std::unique_ptr<TourImprover> improver)
    : constructor(std::move(constructor)), improver(std::move(improver)) {
    if (!this->constructor) {
        throw std::invalid_argument("A tour constructor is required");
    }
}

Tour ConstructAndImproveSolver::solve(const TspInstance& instance) const {
    const Graph& graph = instance.getGraph();
    Tour tour = constructor->build(graph);

    if (improver) {
        tour = improver->improve(graph, std::move(tour));
    }

    return tour;
}

}
