#include "TspParser.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>


std::string trim(const std::string &text) {
    const auto first = text.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}


namespace tsp
{
    TspInstance TspParser::parse(const std::string &filePath) {
        std::ifstream input(filePath);

        if (!input.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        readHeader(input);

        if (dataSection == "NODE_COORD_SECTION") {
            return parseCoordinates(input);
        }

        if (dataSection == "EDGE_WEIGHT_SECTION") {
            if (edgeWeightType != "EXPLICIT" || edgeWeightFormat != "LOWER_DIAG_ROW") {
                throw std::runtime_error("Expected EXPLICIT weights in LOWER_DIAG_ROW format.");
            }

            return parseLowerDiag(input);
        }

        throw std::runtime_error("Unsupported data section.");
    }


    void TspParser::readHeader(std::istream &input)
    {
        std::string line;

        while (std::getline(input, line))
        {
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            if (line == "NODE_COORD_SECTION" || line == "EDGE_WEIGHT_SECTION") {
                dataSection = line;
                break;
            }

            const auto colon = line.find(':');

            if (colon == std::string::npos) {
                throw std::runtime_error("Invalid header line: " + line);
            }

            std::string key = trim(line.substr(0, colon));
            std::string value = trim(line.substr(colon + 1));

            if (key == "NAME") {
                name = value;
            } else if (key == "TYPE") {
                type = value;
            } else if (key == "DIMENSION") {
                std::istringstream number(value);
                std::string extra;

                if (!(number >> dimension) || dimension <= 0 || (number >> extra)) {
                    throw std::runtime_error(
                        "DIMENSION must be a positive integer.");
                }
            } else if (key == "EDGE_WEIGHT_TYPE") {
                edgeWeightType = value;
            } else if (key == "EDGE_WEIGHT_FORMAT") {
                edgeWeightFormat = value;
            }
        }

        if (name.empty()) {
            throw std::runtime_error("Missing NAME.");
        }

        if (type != "TSP") {
            throw std::runtime_error("Expected TYPE: TSP.");
        }

        if (dimension <= 0) {
            throw std::runtime_error("Missing valid DIMENSION.");
        }

        if (dataSection.empty()) {
            throw std::runtime_error("Missing supported data section.");
        }
    }
}
