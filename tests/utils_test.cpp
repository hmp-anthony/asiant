#include <asiant/utils.hpp>
#include <asiant/precision.hpp>

#include "gtest/gtest.h"

using namespace asiant;

TEST(utils, mapping_to_pi_to_minus_pi) {
    real angle = 2 * pi;
    auto result = map_to_range_pi_minus_pi(angle);
    ASSERT_NEAR(result, 0, 0.0001);

    angle = 3 * pi;
    result = map_to_range_pi_minus_pi(angle);
    ASSERT_NEAR(abs(result), pi, 0.0001);

    angle = (3.0 / 2.0) * pi;
    result = map_to_range_pi_minus_pi(angle);
    ASSERT_NEAR(abs(result), 0.5 * pi, 0.0001);
}

TEST(utils, random_number) {
    std::array<real, 10> random_numbers;
    for(int i = 0; i < 10; ++i) {
        random_numbers[i] = random_real(2.0, 10.0);
        bool b1 = random_numbers[i] <= 10.0;
        bool b2 = random_numbers[i] >= 2.0;
        ASSERT_TRUE(b1 && b2);
    }

    
}
