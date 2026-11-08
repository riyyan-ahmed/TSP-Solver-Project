#pragma once
#include <ostream>
#include <string>
#include "Graph.hpp"
#include "TspInstance.hpp"


namespace tsp {
    /** Writes a tour in the TOUR format (NAME, TYPE, DIMENSION, TOUR_SECTION, ids, -1, EOF). */
    class TourWriter {
        public:
            /** Writes the tour to the file at filePath. */
            void write(const TspInstance& instance, const Tour& tour, const std::string& filePath) const;

            /** Writes the tour to a stream (purpose of this is to be used by the tests). */
            void write(const TspInstance& instance, const Tour& tour, std::ostream& output) const;
    };
}
