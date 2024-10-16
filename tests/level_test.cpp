#include <asiant/level.hpp>

#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace asiant;

TEST(level, constructor) {
    std::string level_location = "test_levels/level_0.txt";
    auto l = level(level_location, 800, 600);
    auto g = l.get_graph();
    auto path = dijkstra(*g, l.get_begin(), l.get_end());
    std::vector<int> correct = {0, 1 , 4 , 6};
    ASSERT_EQ(path, correct);

    level_location = "test_levels/level_1.txt";
    l = level(level_location, 800, 600);
    g = l.get_graph();
    path = dijkstra(*g, l.get_begin(), l.get_end());
    correct = { 0, 2, 3, 4, 7, 10, 11, 12 };

    //auto locations = l.get_locations();

    ASSERT_EQ(path, correct);

    level_location = "test_levels/level_2.txt";
    l = level(level_location, 800, 600);
    g = l.get_graph();
    path = dijkstra(*g, l.get_begin(), l.get_end());
    correct = { 0, 3, 4, 5, 10, 15, 16, 17, 18, 24, 29, 30, 35, 40, 46, 47, 48 };
    ASSERT_EQ(path, correct);
}
