#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "TspParser.hpp"
#include "Algorithms.hpp"
#include "TourWriter.hpp"

int main(int argc, char* argv[]) {
    // Check mandatory arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] 
                  << " <instance_path> <output_path> [-m <algorithm>]\n";
        return 1;
    }

    std::string instancePath = argv[1];
    std::string outputPath = argv[2];
    std::string algorithm = "trivial"; // Default method

    // Check for algorithm selection flag 
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m") {
            if (i + 1 < argc) {
                algorithm = argv[i + 1];
                i++; // Skip the algorithm name on next step
            } else {
                std::cerr << "Error: -m flag passed without algorithm name.\n";
                return 1;
            }
        }
    }

    // Run the parser, solver and output tour file
    try {
        // Parse the instance file
        tsp::TspParser parser;
        tsp::TspInstance instance = parser.parse(instancePath);

        // Pick the solver
        std::unique_ptr<tsp::Solver> solver;
        if (algorithm == "trivial") {
            solver = std::make_unique<tsp::TrivialSolver>();
        } else {
            std::cerr << "Error: Unknown algorithm '" << algorithm << "'.\n";
            return 1;
        }

        // Run solver
        tsp::Tour tour = solver->solve(instance);

        // Write the tour file
        tsp::TourWriter writer;
        writer.write(instance, tour, outputPath);

        std::cout << "Successfully solved " << instance.getName() 
                  << " using " << algorithm << ".\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}