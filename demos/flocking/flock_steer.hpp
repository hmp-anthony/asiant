#ifndef DEMO_FLOCK_STEER_HPP
#define DEMO_FLOCK_STEER_HPP

#include <asiant/asiant.hpp>

#include <vector>

using asiant::kinematic;
using asiant::real; 
using asiant::vector;
using asiant::steering;

class flock {
public:
    std::vector<std::shared_ptr<kinematic>> boids;
    std::vector<bool> in_neighbourhood;

    unsigned prepare_neighbourhood(const std::shared_ptr<kinematic> of,
                                   real size, real,
                                   real min_dot_product = -1.0);

    vector get_neighbourhood_center();
    vector get_neighbourhood_average_velocity();
};

class boid_steering_behaviour : public asiant::steering_behaviour {
public:
    std::shared_ptr<flock> the_flock;
    real neighbourhood_size;
    real neighbourhood_minimum_dot_product;
    real max_acceleration;
};

class separation : public boid_steering_behaviour {
public:
    virtual std::shared_ptr<steering> get_steering();
private:
    asiant::flee flee_;
};

class cohesion : public boid_steering_behaviour {
public:
    virtual std::shared_ptr<steering> get_steering();
private:
    asiant::seek seek_;
};

class velocity_match_and_align : public boid_steering_behaviour {
public:
    virtual std::shared_ptr<steering> get_steering();
};

#endif 
