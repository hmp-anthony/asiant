#include <asiant/asiant.hpp>

#include "../application.hpp"
#include "../graphics_utils.hpp"
#include "flock_steer.hpp"


#include <stdio.h>
#include <math.h> 

namespace flocking_info {
    constexpr int number_of_boids_ = 50;
    constexpr real separation_weight = 1.0;
    constexpr real cohesion_weight = 1.0;
    constexpr real vma_weight = 2.0;
}

using namespace graphics_utils;

using asiant::kinematic;
using asiant::blended_steering;
using asiant::random_real;
using asiant::trim_world;

class flocking_demo : public application {
public:
    flocking_demo();
    virtual void display();
    virtual void update();
    virtual bool key(unsigned char key);
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual const char* get_title();
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);
private:
    std::shared_ptr<flock> flock_;

    bool separation_on_;
    bool cohesion_on_;
    bool velocity_match_on_;

    std::shared_ptr<separation> separation_;
    std::shared_ptr<cohesion> cohesion_;
    std::shared_ptr<velocity_match_and_align> velocity_match_and_align_;
    std::shared_ptr<blended_steering> blended_steering_;
};

flocking_demo::flocking_demo() : application(),
    separation_on_(true), cohesion_on_(true), velocity_match_on_(true) {

    static const real accel = (real) 10.0;

    flock_ = std::make_shared<flock>();

    int height = 600;
    int width = 800;

    for(unsigned i = 0; i < flocking_info::number_of_boids_; ++i) {
        auto k = std::make_shared<kinematic>();

        auto position = vector(random_real(width), random_real(height), 0);
        k->set_position(position);

        auto orientation = random_real(asiant::pi);
        k->set_orientation(orientation);

        auto velocity = vector(random_real(2.0), random_real(2.0), 0);
        k->set_velocity(velocity);

        auto rotation = 0;
        k->set_rotation(rotation);

        flock_->boids_.push_back(k);
        flock_->in_neighbourhood_.push_back(true);
    }

    separation_ = std::make_shared<separation>();
    separation_->max_acceleration_ = accel;
    separation_->neighbourhood_size_ = (real)5.0;
    separation_->neighbourhood_minimum_dot_product_ = (real)-1.0;
    separation_->the_flock_ = flock_;

    cohesion_ = std::make_shared<cohesion>();
    cohesion_->max_acceleration_ = accel;
    cohesion_->neighbourhood_size_ = (real)10.0;
    cohesion_->neighbourhood_minimum_dot_product_ = (real)0.0;
    cohesion_->the_flock_ = flock_;

    velocity_match_and_align_ = std::make_shared<velocity_match_and_align>();
    velocity_match_and_align_->max_acceleration_ = accel;
    velocity_match_and_align_->neighbourhood_size_ = (real)15.0;
    velocity_match_and_align_->neighbourhood_minimum_dot_product_ = (real)0.0;
    velocity_match_and_align_->the_flock_ = flock_;

    blended_steering_ = std::make_shared<blended_steering>();

    auto baw_sep = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_sep->behaviour_ = separation_;
    baw_sep->weight_ = flocking_info::separation_weight;
    blended_steering_->behaviours_.push_back(baw_sep);

    auto baw_coh = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_coh->behaviour_ = cohesion_;
    baw_coh->weight_ = flocking_info::cohesion_weight;
    blended_steering_->behaviours_.push_back(baw_coh);

    auto baw_vma = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_vma->behaviour_ = velocity_match_and_align_;
    baw_vma->weight_ = flocking_info::vma_weight;
    blended_steering_->behaviours_.push_back(baw_vma);
}

static const char *default_help[] = {
    "asiant: flocking demo",
    "H - Toggle help.",
    "",
    "Toggle behaviour element:",
    "Q - Separation",
    "W - Cohesion",
    "E - Velocity Match / Align",
};

void flocking_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glColor3f(0.0f, 0.3f, 0.6f);
    for(auto & boid : flock_->boids_) {
        glColor3f(1.0, 0.0, 0.0);
        render_agent(boid);
    }
    display_help();
}


void flocking_demo::update() {
    float duration = asiant::timer::get().last_frame_duration * 0.01f;
    auto boids_ = flock_->boids_;
    auto steer = std::make_shared<steering>();
    for(auto & boid : boids_) {
        blended_steering_->set_character(boid);
        blended_steering_->get_steering(steer);
        boid->integrate(*steer, 0.999, duration);
        boid->update_to_face_velocity();
        boid->trim_max_speed(3.0);
        
        auto pos_in = boid->get_position();
        auto pos_out = trim_world(pos_in, width_, height_);
        boid->set_position(pos_out);
    } 
    application::update();
}

bool flocking_demo::key(unsigned char key) {
    if(application::key(key)) return true;
    real weight = 0;
    switch(key) {
        case 'q':
            separation_on_ = !separation_on_;
            weight = separation_on_ ? flocking_info::separation_weight : 0;
            blended_steering_->behaviours_[0]->weight_ = weight;
            return true;
        case 'w':
            cohesion_on_ = !cohesion_on_;
            weight = cohesion_on_ ? flocking_info::cohesion_weight : 0;
            blended_steering_->behaviours_[1]->weight_ = weight;
            return true;
        case 'e':
            velocity_match_on_ = !velocity_match_on_;
            weight = velocity_match_on_ ? flocking_info::vma_weight : 0;
            blended_steering_->behaviours_[2]->weight_ = weight;
            return true;
    }
    return false;
}

void flocking_demo::get_help_text(const char*** lines, unsigned *line_count) {
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

const char* flocking_demo::get_title()
{
    return default_help[0];
}

unsigned flocking_demo::get_status_count() {
    return 3;
}

const char* flocking_demo::get_status_text(unsigned slot) {
    glColor3f(0.0f, 0.0f, 0.0f);
    
    switch(slot) {
    case 0:
        if (separation_on_) return "Separation On";
        else return "Separation Off";
    case 1:
        if (cohesion_on_) return "Cohesion On";
        else return "Cohesion Off";
    case 2:
        if (velocity_match_on_) return "Velocity Match / Align On";
        else return "Velocity Match / Align Off";
    }
    return "";
}

application* get_application()
{
    return new flocking_demo();
}
