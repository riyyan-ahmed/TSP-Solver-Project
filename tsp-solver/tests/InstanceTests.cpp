// Generated with Claude Code (Claude Sonnet 5.5), 2026-10-14
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include "CoordinatesGraph.hpp"
#include "TspInstance.hpp"

using namespace tsp;

TEST(TspInstance, ExposesNameSizeAndIds) {
    TspInstance instance("demo", {1, 2}, std::make_unique<CoordinatesGraph>(
        std::vector<std::pair<double, double>>{{0, 0}, {1, 1}}));
    EXPECT_EQ(instance.getName(), "demo");
    EXPECT_EQ(instance.size(), 2u);
    EXPECT_EQ(instance.getCityId(0), 1);
    EXPECT_EQ(instance.getCityId(1), 2);
    EXPECT_EQ(instance.getGraph().size(), 2u);
}

TEST(TspInstance, RejectsMissingGraph) {
    EXPECT_THROW(TspInstance("demo", {}, nullptr), std::invalid_argument);
}

TEST(TspInstance, RejectsIdCountMismatch) {
    EXPECT_THROW(TspInstance("demo", {1}, std::make_unique<CoordinatesGraph>(
        std::vector<std::pair<double, double>>{{0, 0}, {1, 1}})), std::invalid_argument);
}

TEST(TspInstance, RejectsOutOfRangeCityId) {
    TspInstance instance("demo", {1}, std::make_unique<CoordinatesGraph>(
        std::vector<std::pair<double, double>>{{0, 0}}));
    EXPECT_THROW(instance.getCityId(1), std::out_of_range);
}
