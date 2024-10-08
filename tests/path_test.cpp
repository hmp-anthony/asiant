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

TEST(path_character, get_position) {
    auto p = std::make_shared<path>();
    line_segment l1(vector(100, 100, 0), vector(400, 100, 0));
    p->add_line_segment(l1);
    path_character pc;
    pc.set_path(p);
    auto v = pc.get_position() - vector(100, 100, 0);
    ASSERT_NEAR(v.square_magnitude(), 0, 0.001);
    pc.update(300.0, 300.0);
    v = pc.get_position() - vector(400, 100, 0); 
    ASSERT_NEAR(v.square_magnitude(), 0, 0.001);
}
