#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Graph.hpp"


namespace tsp {
    class TspInstance {
        std::string name;

        std::vector<int> cityIds;

        std::unique_ptr<Graph> graph;

        public:
            TspInstance(std::string name, std::vector<int> cityIds, std::unique_ptr<Graph> graph);

            const std::string& getName() const;

            std::size_t size() const;

            int getCityId(std::size_t index) const;

            const Graph& getGraph() const;
    };
}
