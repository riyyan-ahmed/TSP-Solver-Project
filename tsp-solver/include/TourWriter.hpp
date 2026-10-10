#pragma once
#include <ostream>
#include <string>
#include "Graph.hpp"
#include "TspInstance.hpp"


namespace tsp {

    // writes a tour in the TOUR format as asked
    /*
    NAME : <name>.tour
    TYPE : TOUR
    DIMENSION : <n>
    TOUR_SECTION
    <city ids, one per line>
    -1
    EOF
    */ 
     
    class TourWriter {
        public:
            // throws std::runtime_error if the file cannot be written
            void write(const TspInstance& instance, const Tour& tour, const std::string& filePath) const;

            // validation for tour does visit every city exactly one
            void write(const TspInstance& instance, const Tour& tour, std::ostream& output) const;
    };
}
