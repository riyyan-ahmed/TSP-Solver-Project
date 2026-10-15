#include "TourWriter.hpp"
#include <fstream>
#include <stdexcept>
#include <vector>


namespace {
    // validity for tour visits so each of the n vertices visits exactly once
    void checkIsPermutation(const tsp::Tour& tour, std::size_t size) {
        if (tour.size() != size) {
            throw std::invalid_argument("The tour must visit every city exactly once.");
        }

        std::vector<bool> visited(size, false);

        for (std::size_t vertex : tour) {
            if (vertex >= size || visited[vertex]) {
                throw std::invalid_argument("The tour must visit every city exactly once.");
            }
            visited[vertex] = true;
        }
    }

    std::string tourName(const std::string& instanceName) {
        const std::string extension = ".tsp";
        std::string base = instanceName;

        if (base.size() > extension.size() && base.compare(base.size() - extension.size(), extension.size(), extension) == 0) {
            base.erase(base.size() - extension.size());
        }

        return base + ".tour";
    }

}


namespace tsp {

void TourWriter::write(const TspInstance& instance, const Tour& tour, const std::string& filePath) const {
    // validated first so that an invalid tour never truncates an existing file
    checkIsPermutation(tour, instance.size());

    std::ofstream output(filePath);

    if (!output.is_open()) {
        throw std::runtime_error("Cannot open output file: " + filePath);
    }

    write(instance, tour, output);

    if (!output) {
        throw std::runtime_error("An error occurred while writing: " + filePath);
    }
}

void TourWriter::write(const TspInstance& instance, const Tour& tour, std::ostream& output) const {
    checkIsPermutation(tour, instance.size());

    output << "NAME : " << tourName(instance.getName()) << '\n';
    output << "TYPE : TOUR\n";
    output << "DIMENSION : " << instance.size() << '\n';
    output << "TOUR_SECTION\n";

    for (std::size_t vertex : tour) {
        output << instance.getCityId(vertex) << '\n';
    }

    output << "-1\n";
    output << "EOF\n";
}

}
