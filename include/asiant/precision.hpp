#ifndef PRECISION_HPP
#define PRECISION_HPP

#include <float.h>
#include <math.h>
#include <numbers>

namespace asiant {
    typedef float real;
    #define real_sqrt sqrtf
    #define real_atan  atan2
    #define real_sin   sinf
    #define real_cos   cosf

    const real pi = std::numbers::pi_v<real>;
}

#endif
