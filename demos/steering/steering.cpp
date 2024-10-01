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
using asiant::steering_behaviour;
using asiant::seek;
using asiant::flee;
using asiant::wander;

class steering_demo : public application {
public:
    steering_demo();

    //virtual bool key(unsigned char key);
    //virtual void update();
    //virtual void display();
    //virtual const char* get_title();
    //virtual void get_help_text(const char*** lines, unsigned *line_count);
    //virtual unsigned get_status_count();
    //virtual const char* get_status_text(unsigned slot);
private:
    // holds the kinematic of the agents.
    std::array<std::shared_ptr<kinematic>, 2> kinematics_;
    std::array<std::shared_ptr<seek>, 2> seeks_;
    std::array<std::shared_ptr<flee>, 2> flees_;
    std::array<std::shared_ptr<wander>, 2> wanders_;

    std::array<std::shared_ptr<steering_behaviour>, 2> current_behaviours_;
    std::array<std::string, 2> status;
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
    status[0] = "Seek";
    current_behaviours_[1] = flees_[1];
    status[1] = "Flee";
    
}


application* get_application() {
    return new steering_demo();
}
