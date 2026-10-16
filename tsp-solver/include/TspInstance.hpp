#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Graph.hpp"

namespace tsp {
    /** This is the instance of TSP and it contains its name, the id of each city and the distance graph. */
    class TspInstance {
        std::string name;
        std::vector<int> cityIds;
        std::unique_ptr<Graph> graph;

        public:
            /** Creates an instance which need a graph and one city id per vertex. */
            TspInstance(std::string name, std::vector<int> cityIds, std::unique_ptr<Graph> graph);

            /** Returns the name of the instance. */
            const std::string& getName() const;

            /** Returns the number of cities. */
            std::size_t size() const;

            /** Returns the id (as written in the file) of the city at this index. */
            int getCityId(std::size_t index) const;

            /** Returns the distance graph. */
            const Graph& getGraph() const;
    };
}
