#pragma once

#include <string>
#include <vector>
#include "TspParser.hpp"

namespace tsp {

    class TspInstance {
        std::string name;

        std::vector<int> cityIds;

        std::vector<std::vector<double>> distances;

        friend class TspParser;

    };
}