#include <asiant/level.hpp>

#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace asiant;

TEST(level, constructor) {
    auto l = level("test_levels/level_1.txt", 800, 600);
    l.print();
}
