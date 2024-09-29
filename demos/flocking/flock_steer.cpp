#include "flock_steer.hpp"

unsigned flock::prepare_neighbourhood(std::shared_ptr<kinematic> of,
                                      real size, real,
                                      real min_dot_product) {
    vector look;
    if(min_dot_product > -1.0f) {
        look = of->get_orientation_as_vector();
    }

    flock result;
    unsigned i = 0, count = 0;
    for(auto& boid : boids) {
        auto k = boid;
        in_neighbourhood[i] = false;
        if(k == of) continue;

    }
};

