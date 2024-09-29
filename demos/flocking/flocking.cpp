#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"
#include "flock_steer.hpp"

#include <stdio.h>
#include <math.h> 

// This is the size of the world in both directions from 0 (i.e. from
// -WORLD_SIZE to +WORLD_SIZE)
#define WORLD_SIZE 50

// This is the gap between grid lines
#define GRID_SIZE 5

// The number of boids in the simulation
#define BOIDS 30

// The relative blend weights
#define SEPARATION_WEIGHT ((aicore::real)1.0)
#define COHESION_WEIGHT ((aicore::real)1.0)
#define VMA_WEIGHT ((aicore::real)2.0)

using asiant::kinematic;

real random_real(real max)
{
   return max * (real(rand()) / RAND_MAX);
}

class flocking_demo : public application {
public:
    flocking_demo();

    virtual void update();
    virtual void display();

private:
    std::shared_ptr<flock> flock_;

    bool separation_on;
    bool cohesion_on;
    bool velocity_match_on;

    std::shared_ptr<separation> separation_;
    std::shared_ptr<cohesion> cohesion_;
    std::shared_ptr<velocity_match_and_align> velocity_match_and_align_;
    std::shared_ptr<blended_steering> blended_steering_;
};

flocking_demo::flocking_demo() : application(), 
                                 separation_on(true),
                                 cohesion_on(true),
                                 velocity_match(true) {
    static const real accel = (real) 10.0;

    flock_ = std::make_shared<flock>();

    for(unsigned i = 0; i < BOIDS; ++i) {
        auto k = std::make_shared<kinematic>();

        auto position = vector(random_real(width), random_real(height), 0);
        k->set_position(position);

        auto orientation = random_real(asiant::pi);
        k->set_orientation(orientation);

        auto velocity = vector(random_real(1.0), random_real(1.0), 0);
        k->set_velocity(velocity);

        auto rotation = 0;
        k->set_rotation(rotation);

        flock_.boids.push_back(k);
    }

    separation_ = std::make_shared<separation>();
    separation_->set_max_acceleration = accel;
    separation_->neighbourhood_size = (real)5.0;
    separation_->neighbourhood_minimum_dot_product = (real)0.0;
    separation_->the_flock = flock_;

    cohesion_ = std::make_shared<cohesion>();
    cohesion_->set_max_acceleration = accel;
    cohesion_->neighbourhood_size = (real)5.0;
    cohesion_->neighbourhood_minimum_dot_product = (real)0.0;
    cohesion_->the_flock = flock_;

    velocity_match_and_align_ = std::make_shared<velocity_match_and_align>();
    velocity_match_and_align_->set_max_acceleration = accel;
    velocity_match_and_align_->neighbourhood_size = (real)5.0;
    velocity_match_and_align_->neighbourhood_minimum_dot_product = (real)0.0;
    velocity_match_and_align_->the_flock = flock_;

    blended_steering_ = std::make_shared<blended_steering>();
    blended_steering::behaviour_and_weight baw_sep(separation_, SEPARATION_WEIGHT);
    blended_steering_->behaviours.push_back(baw_sep);
    blended_steering::behaviour_and_weight baw_coh(cohesion_, COHESION_WEIGHT);
    blended_steering_->behaviours.push_back(baw_coh);
    blended_steering::behaviour_and_weight baw_vma(velocity_match_and_align_, VMA_WEIGHT);
    blended_steering_->behaviours.push_back(baw_vma);
}

application* get_application()
{
    return new flocking_demo();
}