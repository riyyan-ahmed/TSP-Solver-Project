#pragma once
#include <istream>
#include <string>
#include "TspInstance.hpp"


namespace tsp {
    class TspParser {
        std::string name;
        std::string type;
        std::string edgeWeightType;
        std::string edgeWeightFormat;
        std::string dataSection;
        int dimension = 0;

        void readHeader(std::istream& input);

        TspInstance parseLowerDiag(std::istream& input);

        TspInstance parseCoordinates(std::istream& input);

        public:
            TspParser() = default;

            TspInstance parse(const std::string& filePath);

            // parses from any stream, so tests can parse in-memory text.
            TspInstance parse(std::istream& input);

    };
}
