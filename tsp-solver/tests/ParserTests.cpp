// Generated with Claude Code (Claude Sonnet 5.5), 2026-10-14
#include <gtest/gtest.h>
#include <cmath>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include "TspParser.hpp"

#ifndef TSP_INSTANCES_DIR
#error "TSP_INSTANCES_DIR must be defined by the build system"
#endif

using namespace tsp;

namespace {
    TspInstance parseText(const std::string& text) {
        std::istringstream input(text);
        TspParser parser;
        return parser.parse(input);
    }

    // Example instance from the project specification
    const std::string coordText =
        "NAME: example.tsp\n"
        "TYPE: TSP\n"
        "DIMENSION: 5\n"
        "NODE_COORD_SECTION\n"
        "1 565.0 575.0\n"
        "2 25.0 185.0\n"
        "3 345.0 750.0\n"
        "4 945.0 685.0\n"
        "5 845.0 655.0\n"
        "EOF\n";

    const std::string matrixText =
        "NAME: small\n"
        "TYPE: TSP\n"
        "DIMENSION: 4\n"
        "EDGE_WEIGHT_TYPE: EXPLICIT\n"
        "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\n"
        "EDGE_WEIGHT_SECTION\n"
        "0\n5 0\n8 4 0\n6 7 3 0\n"
        "EOF\n";
}

TEST(Parser, ReadsCoordinateInstance) {
    TspInstance instance = parseText(coordText);
    EXPECT_EQ(instance.getName(), "example.tsp");
    EXPECT_EQ(instance.size(), 5u);
    EXPECT_EQ(instance.getCityId(4), 5);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 1),
                     std::sqrt(540.0 * 540.0 + 390.0 * 390.0));
}

TEST(Parser, ReadsLowerDiagInstance) {
    TspInstance instance = parseText(matrixText);
    EXPECT_EQ(instance.size(), 4u);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(3, 2), 3.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(2, 2), 0.0);
}

TEST(Parser, AcceptsSpaceBeforeColonAndWindowsLineEndings) {
    std::string text = "NAME : crlf\r\nTYPE : TSP\r\nDIMENSION : 2\r\n"
                       "NODE_COORD_SECTION\r\n1 0 0\r\n2 3 4\r\nEOF\r\n";
    TspInstance instance = parseText(text);
    EXPECT_EQ(instance.getName(), "crlf");
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 1), 5.0);
}

TEST(Parser, IgnoresCommentsAndUnknownHeaderEntries) {
    std::string text = "NAME: c\nCOMMENT: something : with colon\nTYPE: TSP\nDIMENSION: 1\n"
                       "EDGE_WEIGHT_TYPE: EUC_2D\nNODE_COORD_SECTION\n1 0 0\nEOF\n";
    EXPECT_EQ(parseText(text).size(), 1u);
}

TEST(Parser, AcceptsMissingFinalEof) {
    std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 0 0\n";
    EXPECT_EQ(parseText(text).size(), 1u);
}

TEST(Parser, AcceptsScientificNotationAndNegativeCoordinates) {
    std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 2\nNODE_COORD_SECTION\n"
                       "1 -1.5e+02 2.0E1\n2 0 0\nEOF\n";
    EXPECT_DOUBLE_EQ(parseText(text).getGraph().weight(0, 1), std::sqrt(150.0 * 150.0 + 400.0));
}

TEST(Parser, StoresCoordinatesByIdWhenLinesAreShuffled) {
    std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 3\nNODE_COORD_SECTION\n"
                       "3 6 8\n1 0 0\n2 3 4\nEOF\n";
    TspInstance instance = parseText(text);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 2), 10.0);
}

TEST(Parser, SkipsDisplayDataSectionAfterMatrix) {
    std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                       "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n0\n7 0\n"
                       "DISPLAY_DATA_SECTION\n1 1.0 2.0\n2 3.0 4.0\nEOF\n";
    EXPECT_DOUBLE_EQ(parseText(text).getGraph().weight(0, 1), 7.0);
}

TEST(Parser, ParserCanBeReusedAcrossInstances) {
    TspParser parser;
    std::istringstream first(coordText);
    std::istringstream second(matrixText);
    EXPECT_EQ(parser.parse(first).size(), 5u);
    EXPECT_EQ(parser.parse(second).size(), 4u);
}

TEST(Parser, RejectsMissingFile) {
    TspParser parser;
    EXPECT_THROW(parser.parse("does/not/exist.tsp"), std::runtime_error);
}

TEST(Parser, RejectsEmptyInput) {
    EXPECT_THROW(parseText(""), std::runtime_error);
}

TEST(Parser, RejectsMissingName) {
    EXPECT_THROW(parseText("TYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 0 0\n"), std::runtime_error);
}

TEST(Parser, RejectsWrongType) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: ATSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 0 0\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsMissingOrInvalidDimension) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nNODE_COORD_SECTION\n1 0 0\n"), std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 0\nNODE_COORD_SECTION\n"), std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: -3\nNODE_COORD_SECTION\n"), std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: abc\nNODE_COORD_SECTION\n"), std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 3 4\nNODE_COORD_SECTION\n"), std::runtime_error);
}

TEST(Parser, RejectsHeaderLineWithoutColon) {
    EXPECT_THROW(parseText("NAME: n\nGARBAGE\nTYPE: TSP\n"), std::runtime_error);
}

TEST(Parser, RejectsMissingDataSection) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 1\n"), std::runtime_error);
}

TEST(Parser, RejectsTruncatedCoordinates) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 3\nNODE_COORD_SECTION\n1 0 0\n2 1 1\nEOF\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsNonNumericCoordinate) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 abc 0\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsOutOfRangeAndDuplicateCityIds) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nNODE_COORD_SECTION\n1 0 0\n3 1 1\n"),
                 std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nNODE_COORD_SECTION\n0 0 0\n1 1 1\n"),
                 std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nNODE_COORD_SECTION\n1 0 0\n1 1 1\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsExtraCoordinateLines) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 0 0\n2 1 1\nEOF\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsMatrixWithWrongFormat) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                           "EDGE_WEIGHT_FORMAT: FULL_MATRIX\nEDGE_WEIGHT_SECTION\n0 1\n1 0\nEOF\n"),
                 std::runtime_error);
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_SECTION\n0\n1 0\nEOF\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsInvalidMatrixValues) {
    const std::string head = "NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                             "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n";
    EXPECT_THROW(parseText(head + "0\n-4 0\nEOF\n"), std::runtime_error);   // negative
    EXPECT_THROW(parseText(head + "3\n4 0\nEOF\n"), std::runtime_error);    // non-zero diagonal
    EXPECT_THROW(parseText(head + "0\n4\n"), std::runtime_error);           // truncated
    EXPECT_THROW(parseText(head + "0\nx 0\nEOF\n"), std::runtime_error);    // non numeric
}

TEST(Parser, RejectsTrailingDataAfterMatrix) {
    const std::string head = "NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                             "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n0\n4 0\n";
    EXPECT_THROW(parseText(head + "9\nEOF\n"), std::runtime_error);
    EXPECT_THROW(parseText(head + "EOF\n9\n"), std::runtime_error);
}

TEST(Parser, AcceptsMatrixWithoutFinalEof) {
    const std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                             "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n0\n4 0\n";
    EXPECT_DOUBLE_EQ(parseText(text).getGraph().weight(0, 1), 4.0);
}

TEST(Parser, AcceptsMatrixWrappedOverArbitraryLines) {
    // TSPLIB files do not guarantee one matrix row per line
    const std::string text = "NAME: n\nTYPE: TSP\nDIMENSION: 3\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                             "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n"
                             "0 2\n0 3 4\n0\nEOF\n";
    TspInstance instance = parseText(text);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(2, 0), 3.0);
    EXPECT_DOUBLE_EQ(instance.getGraph().weight(2, 1), 4.0);
}

TEST(Parser, IgnoresBlankLinesAndIndentationInHeaderAndData) {
    const std::string text = "\n  NAME: n\n\n TYPE:TSP\nDIMENSION :2\n  NODE_COORD_SECTION  \n\n"
                             "1 0 0\n\n2 3 4\n\nEOF\n";
    EXPECT_DOUBLE_EQ(parseText(text).getGraph().weight(0, 1), 5.0);
}

TEST(Parser, RejectsUnexpectedDataAfterCoordinates) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 1\nNODE_COORD_SECTION\n1 0 0\nEOF\nmore\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsMatrixShorterThanDimension) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 3\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                           "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n0\n1 0\nEOF\n"),
                 std::runtime_error);
}

TEST(Parser, RejectsNonIntegerMatrixWeight) {
    EXPECT_THROW(parseText("NAME: n\nTYPE: TSP\nDIMENSION: 2\nEDGE_WEIGHT_TYPE: EXPLICIT\n"
                           "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\nEDGE_WEIGHT_SECTION\n0\n1.5 0\nEOF\n"),
                 std::runtime_error);
}

TEST(Parser, ParsesEveryProvidedInstanceWithItsDeclaredDimension) {
    // dimension appears in the file name for the matrix instances
    const std::string dir = std::string(TSP_INSTANCES_DIR) + "/lower_diag/";
    TspParser parser;
    EXPECT_EQ(parser.parse(dir + "gr21.tsp").size(), 21u);
    EXPECT_EQ(parser.parse(dir + "gr24.tsp").size(), 24u);
    EXPECT_EQ(parser.parse(dir + "gr48.tsp").size(), 48u);
}
