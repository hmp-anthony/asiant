#include <asiant/level.hpp>

#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace asiant;

TEST(level, constructor) {
    std::string level_location = "test_levels/level_1.txt";
    auto l = level(level_location, 800, 600);
    l.print();
    ASSERT_EQ(1, 1);
}
