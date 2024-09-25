#include <asiant/utils.hpp>
#include <asiant/precision.hpp>

#include "gtest/gtest.h"

#include <iostream>

using namespace asiant;

TEST(utils, mapping_to_pi_to_minus_pi) {
    real angle = 2 * pi;
    auto result = utils::map_to_range_pi_minus_pi(angle);
    std::cout << result << std::endl;

    angle = 3 * pi;
    result = utils::map_to_range_pi_minus_pi(angle);
    std::cout << result << std::endl;

    angle = (3.0 / 2.0) * pi;
    result = utils::map_to_range_pi_minus_pi(angle);
    std::cout << result << std::endl;
}
