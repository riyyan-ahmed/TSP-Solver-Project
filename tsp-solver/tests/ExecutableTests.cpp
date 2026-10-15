// Runs the real TSPsolver binary, as a user would from the shell
#include <gtest/gtest.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "TspParser.hpp"

#ifndef TSP_SOLVER_EXE
#error "TSP_SOLVER_EXE must be defined by the build system"
#endif
#ifndef TSP_INSTANCES_DIR
#error "TSP_INSTANCES_DIR must be defined by the build system"
#endif

namespace {
    namespace fs = std::filesystem;

    std::string shellQuote(const std::string& path) {
        return "\"" + path + "\"";
    }

    // exit code of the solver, with its output silenced
    int runSolver(const std::string& arguments) {
        const std::string command = shellQuote(TSP_SOLVER_EXE) + " " + arguments + " >/dev/null 2>&1";
        const int status = std::system(command.c_str());
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }

    std::string slurp(const fs::path& path) {
        std::ifstream in(path);
        std::stringstream content;
        content << in.rdbuf();
        return content.str();
    }

    class Executable : public ::testing::Test {
    protected:
        fs::path outPath = fs::temp_directory_path() / "tsp_executable_test.tour";
        std::string instance = std::string(TSP_INSTANCES_DIR) + "/lower_diag/gr17.tsp";

        void SetUp() override { fs::remove(outPath); }
        void TearDown() override { fs::remove(outPath); }
    };
}

TEST_F(Executable, SolvesWithDefaultMethod) {
    ASSERT_EQ(runSolver(shellQuote(instance) + " " + shellQuote(outPath.string())), 0);

    const std::string tour = slurp(outPath);
    EXPECT_EQ(tour.rfind("NAME : gr17.tour\nTYPE : TOUR\nDIMENSION : 17\nTOUR_SECTION\n1\n2\n3\n", 0), 0u);
    EXPECT_NE(tour.find("\n17\n-1\nEOF\n"), std::string::npos);
}

TEST_F(Executable, AcceptsExplicitTrivialMethod) {
    EXPECT_EQ(runSolver(shellQuote(instance) + " " + shellQuote(outPath.string()) + " -m trivial"), 0);
    EXPECT_TRUE(fs::exists(outPath));
}

TEST_F(Executable, FailsOnUnknownAlgorithmWithoutWritingOutput) {
    EXPECT_NE(runSolver(shellQuote(instance) + " " + shellQuote(outPath.string()) + " -m nope"), 0);
    EXPECT_FALSE(fs::exists(outPath));
}

TEST_F(Executable, FailsOnMissingInstance) {
    EXPECT_NE(runSolver("/does/not/exist.tsp " + shellQuote(outPath.string())), 0);
    EXPECT_FALSE(fs::exists(outPath));
}

TEST_F(Executable, FailsWithoutArguments) {
    EXPECT_NE(runSolver(""), 0);
}

TEST_F(Executable, FailsOnUnwritableOutput) {
    EXPECT_NE(runSolver(shellQuote(instance) + " /no/such/dir/out.tour"), 0);
}

TEST_F(Executable, OutputCanBeReadBackAsAPermutation) {
    const std::string coordInstance = std::string(TSP_INSTANCES_DIR) + "/coord/berlin52.tsp";
    ASSERT_EQ(runSolver(shellQuote(coordInstance) + " " + shellQuote(outPath.string())), 0);

    std::istringstream in(slurp(outPath));
    std::string line;
    while (std::getline(in, line) && line != "TOUR_SECTION") {}

    std::vector<bool> seen(53, false);
    int id = 0;
    int count = 0;
    while (in >> id && id != -1) {
        ASSERT_GE(id, 1);
        ASSERT_LE(id, 52);
        EXPECT_FALSE(seen[id]);
        seen[id] = true;
        ++count;
    }
    EXPECT_EQ(count, 52);
}
