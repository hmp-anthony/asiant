#include <asiant/utils.hpp>

#include <random>

asiant::real asiant::map_to_range_pi_minus_pi(real r) {
    auto x = fmod(r + pi, 2 * pi);
    if (x < 0) { x += 2 * pi; }
    return x - pi;
}

asiant::real asiant::random_real(asiant::real min, asiant::real max) {
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

asiant::real asiant::random_real(asiant::real max) {
    return asiant::random_real(0.0, max);
}

asiant::real asiant::random_binomial(asiant::real max) {
    return random_real(max) - random_real(max);
}

asiant::vector asiant::trim_world(asiant::vector& v, asiant::real width, asiant::real height) {
    asiant::real x = v[0];
    asiant::real y = v[1];
    if(v[0] > width) {
        x = 0;
    }
    if(v[0] < 0) {
        x = width;
    }
    if(v[1] > height) {
        y = 0;
    }
    if(v[1] < 0) {
        y = height;
    }
    return asiant::vector(x, y, 0);
}
asiant::vector asiant::trim_world(asiant::vector&& v, asiant::real width, asiant::real height) {
    asiant::real x = v[0];
    asiant::real y = v[1];
    if(v[0] > width) {
        x = 0;
    }
    if(v[0] < 0) {
        x = width;
    }
    if(v[1] > height) {
        y = 0;
    }
    if(v[1] < 0) {
        y = height;
    }
    return asiant::vector(x, y, 0);
}

