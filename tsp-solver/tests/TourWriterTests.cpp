#include <gtest/gtest.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "CoordinatesGraph.hpp"
#include "TourWriter.hpp"

using namespace tsp;

namespace {
    TspInstance makeInstance(const std::string& name, std::vector<int> ids) {
        std::vector<std::pair<double, double>> coordinates;
        for (std::size_t i = 0; i < ids.size(); i++) {
            coordinates.emplace_back(static_cast<double>(i), 0.0);
        }
        return TspInstance(name, std::move(ids), std::make_unique<CoordinatesGraph>(std::move(coordinates)));
    }
}

TEST(TourWriter, WritesSpecificationFormat) {
    TspInstance instance = makeInstance("example.tsp", {1, 2, 3, 4, 5});
    std::ostringstream out;
    TourWriter().write(instance, {0, 2, 4, 1, 3}, out);
    EXPECT_EQ(out.str(), "NAME : example.tour\nTYPE : TOUR\nDIMENSION : 5\nTOUR_SECTION\n" "1\n3\n5\n2\n4\n-1\nEOF\n");
}

TEST(TourWriter, OnlyReplacesTspExtensionAtTheEnd) {
    std::ostringstream a, b;
    TourWriter().write(makeInstance("berlin52", {1}), {0}, a);
    TourWriter().write(makeInstance("my.tsp.tsp", {1}), {0}, b);
    EXPECT_NE(a.str().find("NAME : berlin52.tour\n"), std::string::npos);
    EXPECT_NE(b.str().find("NAME : my.tsp.tour\n"), std::string::npos);
}

TEST(TourWriter, OutputsCityIdsNotIndices) {
    TspInstance instance = makeInstance("ids", {10, 20, 30});
    std::ostringstream out;
    TourWriter().write(instance, {2, 0, 1}, out);
    EXPECT_NE(out.str().find("TOUR_SECTION\n30\n10\n20\n-1\nEOF\n"), std::string::npos);
}

TEST(TourWriter, RejectsInvalidTours) {
    TspInstance instance = makeInstance("n", {1, 2, 3});
    std::ostringstream out;
    TourWriter writer;
    EXPECT_THROW(writer.write(instance, {0, 1}, out), std::invalid_argument);      
    EXPECT_THROW(writer.write(instance, {0, 1, 2, 0}, out), std::invalid_argument);  
    EXPECT_THROW(writer.write(instance, {0, 1, 1}, out), std::invalid_argument);    
    EXPECT_THROW(writer.write(instance, {0, 1, 3}, out), std::invalid_argument);    
    EXPECT_TRUE(out.str().empty());
}

TEST(TourWriter, WritesFile) {
    const auto path = std::filesystem::temp_directory_path() / "tsp_writer_test.tour";
    TourWriter().write(makeInstance("f.tsp", {1, 2}), {1, 0}, path.string());

    std::ifstream in(path);
    std::stringstream content;
    content << in.rdbuf();
    EXPECT_EQ(content.str(), "NAME : f.tour\nTYPE : TOUR\nDIMENSION : 2\nTOUR_SECTION\n2\n1\n-1\nEOF\n");
    std::remove(path.string().c_str());
}

TEST(TourWriter, FailsOnUnwritablePath) {
    EXPECT_THROW(TourWriter().write(makeInstance("n", {1}), {0}, "/no/such/dir/out.tour"), std::runtime_error);
}

TEST(TourWriter, InvalidTourDoesNotTouchExistingFile) {
    const auto path = std::filesystem::temp_directory_path() / "tsp_writer_keep.tour";
    { std::ofstream(path) << "previous content"; }

    EXPECT_THROW(TourWriter().write(makeInstance("n", {1, 2}), {0, 0}, path.string()), std::invalid_argument);

    std::ifstream in(path);
    std::stringstream content;
    content << in.rdbuf();
    EXPECT_EQ(content.str(), "previous content");
    std::remove(path.string().c_str());
}
