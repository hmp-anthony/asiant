#include <asiant/level.hpp>

#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace asiant;

TEST(level, constructor) {
    std::string level_location = "test_levels/level_1.txt";
    auto l = level(level_location, 800, 600);
    auto g = l.get_graph();
    auto path = dijkstra(*g, l.get_begin(), l.get_end());
    for(auto & p : path) {
        std::cout << p << std::endl;
    }
    ASSERT_EQ(1, 1);
}
