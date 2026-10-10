#include "Graph.hpp"
#include <stdexcept>


namespace tsp {

double Graph::tourLength(const Tour& tour) const {
    double length = 0.0;

    for (std::size_t k = 0; k < tour.size(); ++k) {
        const std::size_t from = tour[k];
        const std::size_t to = tour[(k + 1) % tour.size()];

        if (from >= size() || to >= size()) {
            throw std::out_of_range("Tour contains an invalid vertex index.");
        }

        length += weight(from, to);
    }

    return length;
}

}
