#ifndef UTILS_HPP
#define UTILS_HPP

#include <asiant/precision.hpp>
#include <asiant/utils.hpp>

namespace asiant {
    namespace utils {
        real map_to_range_pi_minus_pi(real r) {
            auto x = fmod(r + pi, 2 * pi);
            if (x < 0) { x += 2 * pi; }
            return x - pi;
        }
    };
};

#endif
