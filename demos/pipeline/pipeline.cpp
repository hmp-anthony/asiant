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
    constexpr int number_of_obstacles = 38;
}

class pipeline_demo : public application {
public:
    pipeline_demo();

    //virtual bool key(unsigned char key);
    //virtual void update();
    //virtual void display();
    //virtual const char* get_title();
    //virtual void get_help_text(const char*** lines, unsigned *line_count);
    //virtual unsigned get_status_count();
    //virtual const char* get_status_text(unsigned slot);
private:
    std::shared_ptr<kinematic> kinematic_;
    std::array<std::shared_ptr<sphere>, number_of_obstacles> spheres_;

    std::shared_ptr<steering_pipe> steering_pipe_;
    std::shared_ptr<fixed_goal_targeter> targeter_;
    std::shared_ptr<avoid_spheres_constraint> constraint_;
    std::shared_ptr<basic_actuator> actuator_;

    std::shared_ptr<wander> wander_;
    bool auto_new_goal; 
};

pipeline_demo::pipeline_demo() : application(), auto_new_goal(true) {
    auto accel = (real)50.0;
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(vector((real)5.0, (real)5.0, (real)1.0));
    kinematic_->set_orientation(random_real(pi));
    kinematic_->set_velocity(vector((real)0.0, (real)0.0, (real)0.0));
    kinematic_->set_rotation((real)0.0);

    wander_ = std::make_shared<wander>();
    wander_->set_max_acceleration(accel);
    wander_->set_character(kinematic_);
    wander_->set_max_rotation(1.0);
    wander_->set_max_speed(20.0);
    
    for(unsigned i = 0; i < number_of_obstacles; ++i) {
        spheres_[i] = std::make_shared<sphere>();
        spheres_[i]->center_ = vector(random_real((real)800.0), random_real((real)600.0), 0.0);
        spheres_[i]->radius_ = random_real((real)5.0, (real)20.0);
    }
    
    steering_pipe_ = std::make_shared<steering_pipe>();
    steering_pipe_->set_character(kinematic_);

    targeter_ = std::make_shared<fixed_goal_targeter>();
    steering_pipe_->targeters_.push_back(targeter_);

    constraint_ = std::make_shared<avoid_spheres_constraint>();
    for(unsigned i = 0; i < number_of_obstacles; ++i) {
        
    }
}

application* get_application() {
    return new pipeline_demo();
}

