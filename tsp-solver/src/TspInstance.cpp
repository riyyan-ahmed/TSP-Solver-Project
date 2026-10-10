#include "TspInstance.hpp"
#include <stdexcept>
#include <utility>


namespace tsp {

TspInstance::TspInstance(std::string name, std::vector<int> cityIds, std::unique_ptr<Graph> graph)
    : name(std::move(name)), cityIds(std::move(cityIds)), graph(std::move(graph)) {
    if (!this->graph) {
        throw std::invalid_argument("A TSP instance needs a graph.");
    }

    if (this->cityIds.size() != this->graph->size()) {
        throw std::invalid_argument("There must be exactly one city id per vertex.");
    }
}

const std::string& TspInstance::getName() const {
    return name;
}

std::size_t TspInstance::size() const {
    return graph->size();
}

int TspInstance::getCityId(std::size_t index) const {
    return cityIds.at(index);
}

const Graph& TspInstance::getGraph() const {
    return *graph;
}

}
