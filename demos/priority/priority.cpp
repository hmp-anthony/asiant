#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <array>

#include<iostream>
using namespace graphics_utils;
using namespace asiant;

namespace {
    constexpr int number_of_obstacles = 50;
}

class priority_demo : public application {
public:
    priority_demo();

    virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    virtual const char* get_title();
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);
private:
    std::shared_ptr<kinematic> kinematic_;
    std::array<std::shared_ptr<sphere>, number_of_obstacles> spheres_;
    std::array<std::shared_ptr<avoid_sphere>, number_of_obstacles> avoids_;
    std::shared_ptr<wander> wander_;
    std::shared_ptr<blended_steering> blended_steering_;
    std::shared_ptr<priority_steering> priority_steering_;
    bool is_blended; 
};

priority_demo::priority_demo() : application(), is_blended(false) {
    static const real accel = (real)20.0;
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(vector((real)100.0,(real)100.0,(real)0.0));
    kinematic_->set_orientation(random_real(pi));
    kinematic_->set_velocity(vector((real)0.0,(real)10.0,(real)0.0));
    kinematic_->set_rotation(0.0);

    wander_ = std::make_shared<wander>();
    wander_->set_max_acceleration(accel);
    wander_->set_character(kinematic_);
    wander_->set_max_rotation(2.0);
    wander_->set_max_speed(15.0);

    for(std::size_t i = 0; i < number_of_obstacles; ++i) {
        spheres_[i] = std::make_shared<sphere>();
        spheres_[i]->center_ = vector(random_real(800), random_real(600), (real)0.0);
        spheres_[i]->radius_ = random_real((real)20.0, (real)30.0);

        avoids_[i] = std::make_shared<avoid_sphere>();
        avoids_[i]->set_sphere(spheres_[i]);
        avoids_[i]->set_character(kinematic_);
        avoids_[i]->set_max_acceleration(accel);
        avoids_[i]->set_avoid_margin((real)2.0);
        avoids_[i]->set_max_look_ahead((real)50.0);
    }

    blended_steering_ = std::make_shared<blended_steering>();
    blended_steering_->set_character(kinematic_);

    priority_steering_ = std::make_shared<priority_steering>();
    priority_steering_->set_epsilon((real)0.01);
    priority_steering_->set_character(kinematic_);

    for(std::size_t i = 0; i < number_of_obstacles; ++i) {
        auto baw = std::make_shared<blended_steering::behaviour_and_weight>(avoids_[i], (real)1000.0);
        blended_steering_->behaviours_.push_back(baw);
        priority_steering_->add_behaviour(avoids_[i]);
    }

    auto baw = std::make_shared<blended_steering::behaviour_and_weight>(wander_, 0.0001);
    blended_steering_->behaviours_.push_back(baw);
    priority_steering_->add_behaviour(wander_);

}

bool priority_demo::key(unsigned char key) {
    if (application::key(key)) return true;

    switch(key) {
    case 'B': case 'b':
        is_blended = !is_blended;
        return true;
    }

    return false;
}

void priority_demo::update() {
    auto duration = timer::get().last_frame_duration * 0.005f;
    
    auto wander_steer = std::make_shared<steering>();
    auto steer = std::make_shared<steering>();

    wander_->get_steering(wander_steer);
    steer->clear();

    if(is_blended) {
        blended_steering_->get_steering(steer);
    } else {
        priority_steering_->get_steering(steer);

    }
   
    kinematic_->integrate(*steer, (real)1.0, duration);
    kinematic_->update_to_face_velocity();
    kinematic_->trim_max_speed((real)20.0);
    kinematic_->set_position(trim_world(kinematic_->get_position(), width_, height_));

    glutPostRedisplay();
}

void priority_demo::display() {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    application::set_view();
    // Draw the character
    glColor3f(0.6f, 0.0f, 0.0f);
    render_agent(kinematic_);

    // Draw the obstacles
    glColor3f(0.4f, 0.4f, 0.4f);
    for (std::size_t i = 0; i < number_of_obstacles; ++i) {
		auto s = spheres_[i];	
        glPushMatrix();
        circle(s->center_[0], s->center_[1], s->radius_, 20);
        glPopMatrix();
	}

	// Draw a spot where the avoid steering's target was.
    auto last_used = priority_steering_->get_last_used();
	if (!is_blended && last_used && last_used != wander_)
	{
		glColor3f(1,0,0);
        auto seek_ptr = std::static_pointer_cast<seek>(priority_steering_->get_last_used());
        auto t = seek_ptr->get_target();
        render_spot(t);
	}
	// Draw a spot where the wander wants to go.
    /*
	glColor3f(0,0.5f,0.5f);
    if(wander_->get_target()) { 
	    render_spot(wander_->get_target());
    }
    */
    // Draw the help (the method decides if it should be displayed)
    display_help();
}
static const char *default_help[] = {
    "AI4G: Steering Priority Demo",
    "H - Toggle help.",
    "",
    "B - Toggle behaviour blending / priority",
};

const char* priority_demo::get_title()
{
    return default_help[0];
}

void priority_demo::get_help_text(const char*** lines,
                                        unsigned *line_count)
{
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

unsigned priority_demo::get_status_count()
{
    if (is_blended) return 1;
    else return number_of_obstacles + 1;
}

const char* priority_demo::get_status_text(unsigned slot)
{

    if (is_blended) {
        glColor3f(0.0f, 0.0f, 0.0f);
        return "Blended";
    } else {
        glColor3f(0.8f, 0.8f, 0.8f);
		if (slot < number_of_obstacles)
		{
			if (priority_steering_->get_last_used() == avoids_[slot]) glColor3f(0.6f,0,0);
			return "Avoid Obstacle";
		}
		else
		{
			if (priority_steering_->get_last_used() == wander_) glColor3f(0.6f,0,0);
			return "Wandering";
		}
    }
    return "";
}

application* get_application() {
    return new priority_demo();
}

