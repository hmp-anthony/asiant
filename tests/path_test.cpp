#include <asiant/path.hpp>

#include "gtest/gtest.h"

using namespace asiant;

using asiant::real;
using asiant::vector;
using asiant::line_segment;
using asiant::path;

TEST(path, constructor) {
    path p;

    // define a square path
    line_segment l1(vector(100, 100, 0), vector(400, 100, 0));
    p.add_line_segment(l1);
    line_segment l2(vector(400, 100, 0), vector(400, 400, 0));
    p.add_line_segment(l2);
    line_segment l3(vector(400, 400, 0), vector(100, 400, 0));
    p.add_line_segment(l3);
    line_segment l4(vector(100, 400, 0), vector(100, 100, 0));
    p.add_line_segment(l4);

    auto segments = p.get_line_segments();
    ASSERT_NEAR(segments[0].get_start()[0], 100, 0.0001);
    ASSERT_NEAR(segments[0].get_start()[1], 100, 0.0001);
    ASSERT_NEAR(segments[3].get_start()[0], 100, 0.0001);
    ASSERT_NEAR(segments[3].get_start()[1], 400, 0.0001);
}

#include <iostream>
TEST(path, get_parameter_square) {
    path p;

    // define a square path
    line_segment l1(vector(100, 100, 0), vector(400, 100, 0));
    p.add_line_segment(l1);
    line_segment l2(vector(400, 100, 0), vector(400, 400, 0));
    p.add_line_segment(l2);
    line_segment l3(vector(400, 400, 0), vector(100, 400, 0));
    p.add_line_segment(l3);
    line_segment l4(vector(100, 400, 0), vector(100, 100, 0));
    p.add_line_segment(l4);

    vector v(110, 350, 0);
    auto prmter = p.get_parameter(v);
    ASSERT_NEAR(prmter, 3.16667, 0.0001);
}
