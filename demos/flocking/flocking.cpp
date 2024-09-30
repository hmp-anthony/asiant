#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"
#include "flock_steer.hpp"

#include <stdio.h>
#include <math.h> 
#include <iostream>

// The number of boids in the simulation
#define BOIDS 50

// The relative blend weights
#define SEPARATION_WEIGHT ((real)1.0)
#define COHESION_WEIGHT ((real)1.0)
#define VMA_WEIGHT ((real)2.0)

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < segments + 1; ++n) {
        float const t = 2 * 3.14159 * (float)n / (float)segments;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}

using asiant::kinematic;
using asiant::blended_steering;

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

    std::shared_ptr<separation> separation_;
    std::shared_ptr<cohesion> cohesion_;
    std::shared_ptr<velocity_match_and_align> velocity_match_and_align_;
    std::shared_ptr<blended_steering> blended_steering_;
};

flocking_demo::flocking_demo() : application() {
    static const real accel = (real) 10.0;

    flock_ = std::make_shared<flock>();

    int height = 600;
    int width = 800;

    for(unsigned i = 0; i < BOIDS; ++i) {
        auto k = std::make_shared<kinematic>();

        auto position = vector(random_real(width), random_real(height), 0);
        k->set_position(position);

        auto orientation = random_real(asiant::pi);
        k->set_orientation(orientation);

        auto velocity = vector(random_real(2.0), random_real(2.0), 0);
        k->set_velocity(velocity);

        auto rotation = 0;
        k->set_rotation(rotation);

        flock_->boids.push_back(k);
        flock_->in_neighbourhood.push_back(true);
    }

    separation_ = std::make_shared<separation>();
    separation_->max_acceleration = accel;
    separation_->neighbourhood_size = (real)20.0;
    separation_->neighbourhood_minimum_dot_product = (real)-1.0;
    separation_->the_flock = flock_;

    cohesion_ = std::make_shared<cohesion>();
    cohesion_->max_acceleration = accel;
    cohesion_->neighbourhood_size = (real)20.0;
    cohesion_->neighbourhood_minimum_dot_product = (real)0.0;
    cohesion_->the_flock = flock_;

    velocity_match_and_align_ = std::make_shared<velocity_match_and_align>();
    velocity_match_and_align_->max_acceleration = accel;
    velocity_match_and_align_->neighbourhood_size = (real)25.0;
    velocity_match_and_align_->neighbourhood_minimum_dot_product = (real)0.0;
    velocity_match_and_align_->the_flock = flock_;

    blended_steering_ = std::make_shared<blended_steering>();

    auto baw_sep = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_sep->behaviour = separation_;
    baw_sep->weight = SEPARATION_WEIGHT;
    blended_steering_->behaviours.push_back(baw_sep);

    auto baw_coh = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_coh->behaviour = cohesion_;
    baw_coh->weight = COHESION_WEIGHT;
    blended_steering_->behaviours.push_back(baw_coh);

    auto baw_vma = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_vma->behaviour = velocity_match_and_align_;
    baw_vma->weight = VMA_WEIGHT;
    blended_steering_->behaviours.push_back(baw_vma);
}

void render_agent(std::shared_ptr<kinematic> k)
{
    auto pos = k->get_position();
    auto ori = k->get_orientation();
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glRotatef(ori * 180 / 3.14159, 0, 0, 1);
    glColor3f(1, 0, 0);
    circle(0.0, 0.0, 20.0, 9);
    glColor3f(0, 1, 0);
    circle(0.0, 20.0, 5.0, 20);
    glPopMatrix();
}


void flocking_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glColor3f(0.0f, 0.3f, 0.6f);
    for(auto & boid : flock_->boids) {
        render_agent(boid);
    }
}

vector trim_world(vector& v) {
    real x = v[0];
    real y = v[1];
    if(v[0] > 800) {
        x = 0;
    }
    if(v[0] < 0) {
        x = 800;
    }
    if(v[1] > 600) {
        y = 0;
    }
    if(v[1] < 0) {
        y = 600;
    }
    return vector(x, y, 0);
}

void flocking_demo::update() {
    float duration = asiant::timer::get().last_frame_duration * 0.01f;
    auto boids = flock_->boids;
    auto steer = std::make_shared<steering>();
    for(auto & boid : boids) {
        blended_steering_->set_character(boid);
        blended_steering_->get_steering(steer);
        boid->integrate(*steer, 0.999, duration);
        boid->update_to_face_velocity();
        boid->trim_max_speed(3.0);
        
        auto pos_in = boid->get_position();
        auto pos_out = trim_world(pos_in);
        boid->set_position(pos_out);
    } 
    application::update();
}

application* get_application()
{
    return new flocking_demo();
}
