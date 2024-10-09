#include <asiant/asiant.hpp>

#include "../application.hpp"
#include "../graphics_utils.hpp"
#include "flock_steer.hpp"


#include <stdio.h>
#include <math.h> 
#include <array>

namespace {
    constexpr int number_of_boids = 50;
    constexpr real separation_weight = 100.0;
    constexpr real cohesion_weight = 0.1;
    constexpr real vma_weight = 0.5;
    constexpr real follow_path_weight = 0.01;
}

using namespace graphics_utils;
using namespace asiant;

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

    std::shared_ptr<path_character> path_character_;
    std::array<std::shared_ptr<follow_path_seek>, number_of_boids> follow_path_;

    std::shared_ptr<separation> separation_;
    std::shared_ptr<cohesion> cohesion_;
    std::shared_ptr<velocity_match_and_align> velocity_match_and_align_;

    std::shared_ptr<blended_steering> blended_steering_;
    std::shared_ptr<priority_steering> priority_steering_;
    bool is_priority;
};

flocking_demo::flocking_demo() : application(),
    separation_on_(true), cohesion_on_(true), velocity_match_on_(true), is_priority(false) {

    static const real accel = (real) 10.0;

    flock_ = std::make_shared<flock>();

    int height = 600;
    int width = 800;

    // create path character
    path_character_ = std::make_shared<path_character>();

    // create and set path for path_character to follow
    auto p = std::make_shared<asiant::path>();
    asiant::line_segment l1(asiant::vector(100, 100, 0), asiant::vector(700, 100, 0));
    p->add_line_segment(l1);
    asiant::line_segment l2(asiant::vector(700, 100, 0), asiant::vector(700, 600, 0));
    p->add_line_segment(l2);
    asiant::line_segment l3(asiant::vector(700, 600, 0), asiant::vector(100, 600, 0));
    p->add_line_segment(l3);
    asiant::line_segment l4(asiant::vector(100, 600, 0), asiant::vector(100, 100, 0));
    p->add_line_segment(l4);
    asiant::line_segment l5(asiant::vector(100, 100, 0), asiant::vector(700, 600, 0));
    p->add_line_segment(l5);
    asiant::line_segment l6(asiant::vector(700, 600, 0), asiant::vector(100, 600, 0));
    p->add_line_segment(l6);
    asiant::line_segment l7(asiant::vector(100, 600, 0), asiant::vector(100, 100, 0));
    p->add_line_segment(l7);
    path_character_->set_path(p);

   
    for(unsigned i = 0; i < number_of_boids; ++i) {
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

        follow_path_[i] = std::make_shared<follow_path_seek>();
        follow_path_[i]->set_path_character(path_character_);
        follow_path_[i]->set_max_acceleration(20.0);
        follow_path_[i]->set_target_radius(400.0);
        follow_path_[i]->set_velocity_radius(500.0);
        follow_path_[i]->set_time_to_target(0.4);
        follow_path_[i]->set_max_speed(20.0);
    }

    separation_ = std::make_shared<separation>();
    separation_->max_acceleration_ = accel;
    separation_->neighbourhood_size_ = (real)50.0;
    separation_->neighbourhood_minimum_dot_product_ = (real)-1.0;
    separation_->the_flock_ = flock_;

    cohesion_ = std::make_shared<cohesion>();
    cohesion_->max_acceleration_ = accel;
    cohesion_->neighbourhood_size_ = (real)20.0;
    cohesion_->neighbourhood_minimum_dot_product_ = (real)0.0;
    cohesion_->the_flock_ = flock_;

    velocity_match_and_align_ = std::make_shared<velocity_match_and_align>();
    velocity_match_and_align_->max_acceleration_ = accel;
    velocity_match_and_align_->neighbourhood_size_ = (real)100.0;
    velocity_match_and_align_->neighbourhood_minimum_dot_product_ = (real)0.0;
    velocity_match_and_align_->the_flock_ = flock_;

    blended_steering_ = std::make_shared<blended_steering>();
    priority_steering_ = std::make_shared<priority_steering>();
    priority_steering_->set_epsilon((real)0.1);

    auto baw_sep = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_sep->behaviour_ = separation_;
    baw_sep->weight_ = separation_weight;
    blended_steering_->behaviours_.push_back(baw_sep);
    priority_steering_->add_behaviour(separation_);

    auto baw_coh = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_coh->behaviour_ = cohesion_;
    baw_coh->weight_ = cohesion_weight;
    blended_steering_->behaviours_.push_back(baw_coh);
    priority_steering_->add_behaviour(cohesion_);

    auto baw_vma = std::make_shared<blended_steering::behaviour_and_weight>();
    baw_vma->behaviour_ = velocity_match_and_align_;
    baw_vma->weight_ = vma_weight;
    blended_steering_->behaviours_.push_back(baw_vma);
    priority_steering_->add_behaviour(velocity_match_and_align_);

    for(unsigned i = 0; i < number_of_boids; ++i) {
        auto baw_follow = std::make_shared<blended_steering::behaviour_and_weight>();
        baw_follow->behaviour_ = follow_path_[i];
        baw_follow->weight_ = follow_path_weight;
        blended_steering_->behaviours_.push_back(baw_follow);
        priority_steering_->add_behaviour(follow_path_[i]);
    }
}

static const char *default_help[] = {
    "asiant: flocking demo",
    "H - Toggle help.",
    "",
    "Toggle behaviour element:",
    "Q - Separation",
    "W - Cohesion",
    "E - Velocity Match / Align",
    "B - Toggle behaviour blending / priority",
    "In priority mode, all behaviour elements are present."
};

void flocking_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glColor3f(0.0f, 0.3f, 0.6f);
    for(auto & boid : flock_->boids_) {
        glColor3f(1.0, 0.0, 0.0);
        render_agent(boid);
    }

    glColor3f(0.4f, 0.3f, 0.1f);
    render_spot(path_character_->get_position());
    display_help();
}


void flocking_demo::update() {
    float duration = asiant::timer::get().last_frame_duration * 0.01f;
    auto boids_ = flock_->boids_;
    auto steer = std::make_shared<steering>();
    for(auto & boid : boids_) {
        if(is_priority) {
            priority_steering_->set_character(boid);
            priority_steering_->get_steering(steer);
        } else {
            blended_steering_->set_character(boid);
            blended_steering_->get_steering(steer);
        }
        boid->integrate(*steer, 0.8, duration);
        boid->update_to_face_velocity();
        boid->trim_max_speed(3.0);
        
        path_character_->update(0.8, duration);
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
            weight = separation_on_ ? separation_weight : 0;
            blended_steering_->behaviours_[0]->weight_ = weight;
            return true;
        case 'w':
            cohesion_on_ = !cohesion_on_;
            weight = cohesion_on_ ? cohesion_weight : 0;
            blended_steering_->behaviours_[1]->weight_ = weight;
            return true;
        case 'e':
            velocity_match_on_ = !velocity_match_on_;
            weight = velocity_match_on_ ? vma_weight : 0;
            blended_steering_->behaviours_[2]->weight_ = weight;
            return true;
        case 'b':
            is_priority = !is_priority;
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
    if(is_priority) return 1;
    else return 3;
}

const char* flocking_demo::get_status_text(unsigned slot) {
    glColor3f(0.0f, 0.0f, 0.0f);
    
    if(is_priority) {
        return "priority";
    } else {
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
    }
    return "";
}

application* get_application()
{
    return new flocking_demo();
}
