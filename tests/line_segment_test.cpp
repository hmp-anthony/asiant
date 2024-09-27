#include <asiant/line_segment.hpp>

#include "gtest/gtest.h"

using namespace asiant;

using asiant::real;
using asiant::vector;
using asiant::line_segment;

TEST(line_segment, constructor) {
    auto ls = line_segment(vector(1,1,0), vector(1,4,0));
    auto s = ls.get_start();
    auto e = ls.get_end();

    ASSERT_EQ(s[0], 1.0);
    ASSERT_EQ(s[1], 1.0);
    ASSERT_EQ(s[2], 0.0);

    ASSERT_EQ(e[0], 1.0);
    ASSERT_EQ(e[1], 4.0);
    ASSERT_EQ(e[2], 0.0);
}

TEST(line_segment, bracket_operator) {
    auto ls = line_segment(vector(0,0,0), vector(1,0,0));

    auto start = ls[0.0];
    ASSERT_NEAR(start[0], 0, 0.0001);
    ASSERT_NEAR(start[1], 0, 0.0001);
    ASSERT_NEAR(start[2], 0, 0.0001);

    auto end = ls[1.0];
    ASSERT_NEAR(end[0], 1, 0.0001);
    ASSERT_NEAR(end[1], 0, 0.0001);
    ASSERT_NEAR(end[2], 0, 0.0001);

    auto mid_point = ls[0.5];
    ASSERT_NEAR(mid_point[0], 0.5, 0.0001);
    ASSERT_NEAR(mid_point[1], 0, 0.0001);
    ASSERT_NEAR(mid_point[2], 0, 0.0001);

    // here we want to go beyond the end points
    auto beyond_begin = ls[-0.5];
    ASSERT_NEAR(beyond_begin[0], -0.5, 0.0001);
    ASSERT_NEAR(beyond_begin[1], 0, 0.0001);
    ASSERT_NEAR(beyond_begin[2], 0, 0.0001);

    auto beyond_end = ls[1.5];
    ASSERT_NEAR(beyond_end[0], 1.5, 0.0001);
    ASSERT_NEAR(beyond_end[1], 0, 0.0001);
    ASSERT_NEAR(beyond_end[2], 0, 0.0001);
}

TEST(line_segment, at) {
    auto ls = line_segment(vector(0,0,0), vector(1,0,0));

    auto start = ls.at(0.0);
    ASSERT_NEAR(start[0], 0, 0.0001);
    ASSERT_NEAR(start[1], 0, 0.0001);
    ASSERT_NEAR(start[2], 0, 0.0001);

    auto end = ls.at(1.0);
    ASSERT_NEAR(end[0], 1, 0.0001);
    ASSERT_NEAR(end[1], 0, 0.0001);
    ASSERT_NEAR(end[2], 0, 0.0001);

    auto mid_point = ls.at(0.5);
    ASSERT_NEAR(mid_point[0], 0.5, 0.0001);
    ASSERT_NEAR(mid_point[1], 0, 0.0001);
    ASSERT_NEAR(mid_point[2], 0, 0.0001);

    // here we want to go beyond the end points
    auto beyond_begin = ls.at(-0.5);
    ASSERT_NEAR(beyond_begin[0], 0, 0.0001);
    ASSERT_NEAR(beyond_begin[1], 0, 0.0001);
    ASSERT_NEAR(beyond_begin[2], 0, 0.0001);

    auto beyond_end = ls.at(1.5);
    ASSERT_NEAR(beyond_end[0], 1, 0.0001);
    ASSERT_NEAR(beyond_end[1], 0, 0.0001);
    ASSERT_NEAR(beyond_end[2], 0, 0.0001);
}

