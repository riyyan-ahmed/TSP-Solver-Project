#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Algorithms.hpp"
#include "TourWriter.hpp"
#include "TspParser.hpp"



namespace {
    /** Prints how to call the program. */
    void printUsage(const char* program) {
        std::cerr << "Usage: " << program << " <instance_path> <output_path> [-m <algorithm>]\n" << "Algorithms: trivial (default)\n";
    }

    /** Returns the solver for an algorithm name, or nullptr if it does not exist. */
    std::unique_ptr<tsp::Solver> makeSolver(const std::string& algorithm) {
        if (algorithm == "trivial") {
            return std::make_unique<tsp::TrivialSolver>();
        }
        return nullptr;
    }
}

/** Reads the instance, solves it with the chosen method and writes the tour file. */
int main(int argc, char* argv[]) {
    std::vector<std::string> paths;
    std::string algorithm = "trivial";

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "-m") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -m must be followed by an algorithm name\n";
                printUsage(argv[0]);
                return 1;
            }
            algorithm = argv[++i];
        } else if (arg.size() > 1 && arg[0] == '-') {
            std::cerr << "Error: unknown option '" << arg << "'.\n";
            printUsage(argv[0]);
            return 1;
        } else {
            paths.push_back(arg);
        }
    }

    if (paths.size() != 2) {
        std::cerr << "Error: expected an instance path and an output path\n";
        printUsage(argv[0]);
        return 1;
    }

    std::unique_ptr<tsp::Solver> solver = makeSolver(algorithm);
    if (!solver) {
        std::cerr << "Error: unknown or not yet implemented algorithm '" << algorithm << "'.\n";
        printUsage(argv[0]);
        return 1;
    }

    try {
        tsp::TspParser parser;
        tsp::TspInstance instance = parser.parse(paths[0]);

        tsp::Tour tour = solver->solve(instance);

        tsp::TourWriter writer;
        writer.write(instance, tour, paths[1]);

        std::cout << "Successfully solved " << instance.getName() << " using " << algorithm << ".\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
