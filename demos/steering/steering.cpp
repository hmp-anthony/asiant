#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <array>

#include<iostream>
using namespace graphics_utils;

using asiant::real;
using asiant::vector;
using asiant::kinematic;
using asiant::steering;
using asiant::steering_behaviour;
using asiant::seek;
using asiant::flee;
using asiant::wander;

class steering_demo : public application {
public:
    steering_demo();

    virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    virtual const char* get_title();
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);
private:
    // holds the kinematic of the agents.
    std::array<std::shared_ptr<kinematic>, 2> kinematics_;
    std::array<std::shared_ptr<seek>, 2> seeks_;
    std::array<std::shared_ptr<flee>, 2> flees_;
    std::array<std::shared_ptr<wander>, 2> wanders_;

    std::array<std::shared_ptr<steering_behaviour>, 2> current_behaviours_;
    std::array<std::string, 2> status_;
};

steering_demo::steering_demo() : application() {
    static const real accel = (real)20.0;

    kinematics_[0] = std::make_shared<kinematic>();
    kinematics_[1] = std::make_shared<kinematic>();
   
    auto pos = vector(10.0, 10.0, 0.0);
    auto vel = vector(0.0, 0.0, 0.0);
    kinematics_[0]->set_position(pos);
    kinematics_[0]->set_orientation(2.1);
    kinematics_[0]->set_velocity(vel);
    kinematics_[0]->set_rotation(0.0);

    pos = vector(500.0, 500.0, 0.0);
    kinematics_[1]->set_position(pos);
    kinematics_[1]->set_orientation(0.75);
    kinematics_[1]->set_velocity(vel);
    kinematics_[1]->set_rotation(0.0);

    seeks_[0] = std::make_shared<seek>();
    seeks_[1] = std::make_shared<seek>();
    flees_[0] = std::make_shared<flee>();
    flees_[1] = std::make_shared<flee>();
    wanders_[0] = std::make_shared<wander>();
    wanders_[1] = std::make_shared<wander>();

    for(unsigned i = 0; i < 2; ++i) {
        auto target = std::make_shared<vector>(kinematics_[1-i]->get_position());
        seeks_[i]->set_character(kinematics_[i]);
        seeks_[i]->set_target(target);
        seeks_[i]->set_max_acceleration(accel);
        
        flees_[i]->set_character(kinematics_[i]);
        flees_[i]->set_target(target);
        flees_[i]->set_max_acceleration(accel);

        wanders_[i]->set_character(kinematics_[i]);
        wanders_[i]->set_max_acceleration(accel);
        wanders_[i]->set_volatility((real)20.0);
        wanders_[i]->set_turn_speed((real)2.0);
    }
    
    current_behaviours_[0] = seeks_[0];
    status_[0] = "Seek";
    current_behaviours_[1] = flees_[1];
    status_[1] = "Flee";
    
}


bool steering_demo::key(unsigned char key)
{
    if (application::key(key)) return true;

    switch(key) {
    case 'Q': case 'q':
        current_behaviours_[0] = nullptr;
        status_[0] = "Stationary";
        return true;
    case 'W': case 'w':
        current_behaviours_[0] = seeks_[0];
        status_[0] = "Seek";
        return true;
    case 'E': case 'e':
        current_behaviours_[0] = flees_[0];
        status_[0] = "Flee";
        return true;
	case 'R': case 'r':
		current_behaviours_[0] = wanders_[0];
		status_[0] = "Wander";
		return true;

    case 'A': case 'a':
        current_behaviours_[1] = nullptr;
        status_[1] = "Stationary";
        return true;
    case 'S': case 's':
        current_behaviours_[1] = seeks_[1];
        status_[1] = "Seek";
        return true;
    case 'D': case 'd':
        current_behaviours_[1] = flees_[1];
        status_[1] = "Flee";
        return true;
	case 'F': case 'f':
		current_behaviours_[1] = wanders_[1];
		status_[1] = "Wander";
		return true;
    }

    return false;
}

void steering_demo::update()
{
    float duration = asiant::timer::get().last_frame_duration * 0.005f;
    auto steer = std::make_shared<steering>();;

    for (unsigned i = 0; i < 2; i++) {

        // Update the steering if we have a movement algorithm
        if (current_behaviours_[i]) current_behaviours_[i]->get_steering(steer);
        else steer->clear();

        // Update the kinematic
        kinematics_[i]->integrate(*steer, (real)1.0, duration);
        kinematics_[i]->update_to_face_velocity();

        // Check for maximum speed
        kinematics_[i]->trim_max_speed((real)25.0);

        // Keep in bounds of the world
        auto pos_in = kinematics_[i]->get_position();
        auto pos_out = trim_world(pos_in, width_, height_);
        kinematics_[i]->set_position(pos_out);
    }

    glutPostRedisplay();
}

void steering_demo::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.6f, 0.0f, 0.0f);
    render_agent(kinematics_[0]);
    glColor3f(0.0f, 0.6f, 0.0f);
    render_agent(kinematics_[1]);

    display_help();
}

static const char *default_help[] = {
    "AI4G: Steering Demo",
    "H - Toggle help.",
    "",
    "Red character:",
    "Q - Stationary",
    "W - Seek",
    "E - Flee",
    "R - Wander",
    "",
    "Green character:",
    "A - Stationary",
    "S - Seek",
    "D - Flee",
	"F - Wander",
};

const char* steering_demo::get_title()
{
    return default_help[0];
}

void steering_demo::get_help_text(const char*** lines, unsigned *line_count)
{
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

unsigned steering_demo::get_status_count()
{
    return 2;
}

const char* steering_demo::get_status_text(unsigned slot)
{
    // Set the color
    switch(slot)
    {
    case 0:
        glColor3f(0.6f, 0.0f, 0.0f);
        break;
    case 1:
        glColor3f(0.0f, 0.6f, 0.0f);
        break;
    }

    // Return the text
    return status_[slot].c_str();
}

application* get_application() {
    return new steering_demo();
}

