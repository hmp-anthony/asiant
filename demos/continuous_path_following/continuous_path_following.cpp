#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <array>

#include<iostream>
using namespace graphics_utils;
using namespace asiant;

class continuous_path_following_demo : public application {
public:
    continuous_path_following_demo();

    virtual void update();
    virtual void display();
private:
    // holds the kinematic of the agents.
    std::vector<vector> control_points_;
    std::shared_ptr<continuous_path_catmull> catmull_;
    std::vector<vector> catmull_path_;

    std::shared_ptr<kinematic> kinematic_;
    std::shared_ptr<vector> target_;
    std::shared_ptr<seek_with_velocity_radius> seek_;

    int path_location_;
    real time_passed_;
};

continuous_path_following_demo::continuous_path_following_demo() : application() {

    control_points_.push_back(vector(100, 100, 0));
    control_points_.push_back(vector(700, 100, 0));
    control_points_.push_back(vector(400, 400, 0));
    control_points_.push_back(vector(700, 550, 0));
    control_points_.push_back(vector(100, 600, 0));
    
    catmull_ = std::make_shared<continuous_path_catmull>(control_points_, true);
    catmull_path_ = catmull_->get_path();

    path_location_ = 0;
    time_passed_ = 0;
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(catmull_path_[0]);
    kinematic_->set_velocity(vector(0,0,0));

    target_ = std::make_shared<vector>();
    *target_ = catmull_path_[path_location_];

    seek_ = std::make_shared<seek_with_velocity_radius>();
    seek_->set_character(kinematic_);
    seek_->set_target(target_);
    seek_->set_max_acceleration(1000.0);
    seek_->set_target_radius(10.0);
    seek_->set_velocity_radius(150);
    seek_->set_time_to_target(0.0000001);
    seek_->set_max_speed(1000.0);
}

#include<iostream>
void continuous_path_following_demo::update() {    
    // find the displacement vector.
    vector delta;
    if(path_location_ < catmull_path_.size() - 1) {
        delta = catmull_path_[path_location_ + 1] - catmull_path_[path_location_];
    } else {
        delta = catmull_path_[0] - catmull_path_.back();
    }
    auto delta_distance = delta.magnitude();

    float duration = (float)asiant::timer::get().last_frame_duration * 0.001f;
    time_passed_ += duration;
    real velocity = 135;
    if((time_passed_ * velocity) > delta_distance) {
        path_location_++;
        time_passed_ = 0;
    }
    if(path_location_ > catmull_path_.size()) {
        path_location_ = 0;
    }

    *target_ = catmull_path_[path_location_];

    auto steer = std::make_shared<steering>();
    seek_->get_steering(steer);
    kinematic_->update(steer, 100.0, duration);
    kinematic_->update_to_face_velocity();

    application::update();
}

void continuous_path_following_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glColor3f(0.0f, 0.0f, 0.0f);
    render_agent(kinematic_);

    glColor3f(1.0f, 1.0f, 1.0f);
    render_spot(catmull_path_[path_location_]);

    glColor3f(0.0f, 0.6f, 0.0f);
    for(auto & v : control_points_) {
        render_spot(v);
    }

    glColor3f(0.6f, 0.0f, 1.0f);
    for(auto & v : catmull_path_) {
        render_spot(v);
    }
    
}

application* get_application() {
    return new continuous_path_following_demo();
}

