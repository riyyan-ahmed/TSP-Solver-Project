#pragma once
#include <istream>
#include <string>
#include "TspInstance.hpp"


namespace tsp {
    /** Read a TSPLIB .tsp file and used that to build a TspInstance. */
    class TspParser {
        std::string name;
        std::string type;
        std::string edgeWeightType;
        std::string edgeWeightFormat;
        std::string dataSection;
        int dimension = 0;

        /** Internal functions */
        /** Read the header lines up to the data section and checks them. */
        void readHeader(std::istream& input);

        /** Read an EDGE_WEIGHT_SECTION in LOWER_DIAG_ROW format. */
        TspInstance parseLowerDiag(std::istream& input);

        /** Read a NODE_COORD_SECTION ("<id> <x> <y>" per city). */
        TspInstance parseCoordinates(std::istream& input);

        public:
            TspParser() = default;

            /** Parses the file from the given filePath. */
            TspInstance parse(const std::string& filePath);

            /** Parses from a stream (purpose is to be used by the tests). */
            TspInstance parse(std::istream& input);
    };
}
