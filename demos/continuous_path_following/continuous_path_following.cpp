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
    std::shared_ptr<continuous_path_bezier> bezier_path_;
    std::vector<vector> control_points_;
    std::vector<vector> path_;
};

continuous_path_following_demo::continuous_path_following_demo() : application() {

    control_points_.push_back(vector(100, 100, 0));
    control_points_.push_back(vector(300, 100, 0));
    control_points_.push_back(vector(300, 300, 0));
    control_points_.push_back(vector(100, 300, 0));
    control_points_.push_back(vector(500, 500, 0));
    control_points_.push_back(vector(500, 600, 0));
    control_points_.push_back(vector(600, 600, 0));
    control_points_.push_back(vector(700, 300, 0));

    bezier_path_ = std::make_shared<continuous_path_bezier>(control_points_);
    path_ = bezier_path_->get_path();
}

void continuous_path_following_demo::update() {    
}

void continuous_path_following_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.6f, 0.0f, 0.0f);
    for(auto & v : path_) {
        render_spot(v);
    }
    glColor3f(0.0f, 0.6f, 0.0f);
    for(auto & v : control_points_) {
        render_spot(v);
    }
}

application* get_application() {
    return new continuous_path_following_demo();
}

