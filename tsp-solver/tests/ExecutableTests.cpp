// Take help from Claude (Claude Sonnet 5.5), 2026-10-13 & 2026-11-07 -> We used AI to write slurp, and some part of Test F
#include <gtest/gtest.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/wait.h>
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

    // true if the tour section of the file lists each city id from 1 to n exactly once
    bool tourIdsArePermutation(const fs::path& path, int n) {
        std::istringstream in(slurp(path));
        std::string line;
        while (std::getline(in, line) && line != "TOUR_SECTION") {}

        std::vector<bool> seen(n + 1, false);
        int id = 0;
        int count = 0;
        while (in >> id && id != -1) {
            if (id < 1 || id > n || seen[id]) {
                return false;
            }
            seen[id] = true;
            ++count;
        }
        return count == n;
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

TEST_F(Executable, SolvesWithEveryMethod) {
    // burma14 is small enough for held-karp
    const std::string coordInstance = std::string(TSP_INSTANCES_DIR) + "/coord/burma14.tsp";

    for (const char* method : {"trivial", "held-karp", "mst", "mst-2opt", "nn", "nn-2opt"}) {
        SCOPED_TRACE(method);
        fs::remove(outPath);
        ASSERT_EQ(runSolver(shellQuote(coordInstance) + " " + shellQuote(outPath.string()) + " -m " + method), 0);
        EXPECT_TRUE(tourIdsArePermutation(outPath, 14));
    }
}

TEST_F(Executable, AcceptsTheMethodBeforeOrBetweenThePaths) {
    EXPECT_EQ(runSolver("-m trivial " + shellQuote(instance) + " " + shellQuote(outPath.string())), 0);
    EXPECT_TRUE(fs::exists(outPath));

    fs::remove(outPath);
    EXPECT_EQ(runSolver(shellQuote(instance) + " -m trivial " + shellQuote(outPath.string())), 0);
    EXPECT_TRUE(fs::exists(outPath));
}

TEST_F(Executable, FailsWithTooFewArguments) {
    EXPECT_NE(runSolver(""), 0);
    EXPECT_NE(runSolver(shellQuote(instance)), 0);
}

TEST_F(Executable, FailsOnInvalidArgumentsWithoutWritingOutput) {
    const std::string paths = shellQuote(instance) + " " + shellQuote(outPath.string());
    EXPECT_NE(runSolver(paths + " -m nope"), 0);       // unknown method
    EXPECT_NE(runSolver(paths + " -m"), 0);            // missing method name
    EXPECT_NE(runSolver(paths + " -x trivial"), 0);    // unknown option
    EXPECT_NE(runSolver(paths + " extra"), 0);         // extra argument
    EXPECT_FALSE(fs::exists(outPath));
}

TEST_F(Executable, FailsOnMissingInstance) {
    EXPECT_NE(runSolver("/does/not/exist.tsp " + shellQuote(outPath.string())), 0);
    EXPECT_FALSE(fs::exists(outPath));
}

TEST_F(Executable, FailsOnUnwritableOutput) {
    EXPECT_NE(runSolver(shellQuote(instance) + " /no/such/dir/out.tour"), 0);
}

TEST_F(Executable, HeldKarpFailsOnLargeInstanceWithoutWritingOutput) {
    const std::string coordInstance = std::string(TSP_INSTANCES_DIR) + "/coord/berlin52.tsp";
    EXPECT_NE(runSolver(shellQuote(coordInstance) + " " + shellQuote(outPath.string()) + " -m held-karp"), 0);
    EXPECT_FALSE(fs::exists(outPath));
}
