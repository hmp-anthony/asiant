#ifndef UTILS_HPP
#define UTILS_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>

namespace asiant {
    real map_to_range_pi_minus_pi(real r);
    real random_real(real max);
    real random_real(real min, real max);
    real random_binomial(real max);
    vector trim_world(vector& v, real width, real height);
    vector trim_world(vector&& v, real width, real height);
};

#endif
