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
    std::vector<std::shared_ptr<kinematic>> boids_;
    std::vector<bool> in_neighbourhood_;

    unsigned prepare_neighbourhood(const std::shared_ptr<kinematic> of,
                                   real size,
                                   real min_dot_product = -1.0);

    vector get_neighbourhood_center();
    vector get_neighbourhood_average_velocity();
};

class boid_steering_behaviour : public asiant::steering_behaviour {
public:
    std::shared_ptr<flock> the_flock_;
    real neighbourhood_size_;
    real neighbourhood_minimum_dot_product_;
    real max_acceleration_;
};

class separation : public boid_steering_behaviour {
public:
    virtual void get_steering(std::shared_ptr<steering> steer);
private:
    asiant::flee flee_;
};

class cohesion : public boid_steering_behaviour {
public:
    virtual void get_steering(std::shared_ptr<steering> steer);
private:
    asiant::seek seek_;
};

class velocity_match_and_align : public boid_steering_behaviour {
public:
    virtual void get_steering(std::shared_ptr<steering> steer);
};

#endif 
