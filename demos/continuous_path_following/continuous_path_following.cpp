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
    std::shared_ptr<continuous_path_bezier> bezier_;
    std::vector<vector> bezier_path_;
    std::shared_ptr<continuous_path_catmull> catmull_;
    std::vector<vector> catmull_path_;
};

continuous_path_following_demo::continuous_path_following_demo() : application() {

    control_points_.push_back(vector(100, 100, 0));
    control_points_.push_back(vector(300, 100, 0));
    control_points_.push_back(vector(400, 400, 0));
    control_points_.push_back(vector(600, 600, 0));
    control_points_.push_back(vector(100, 600, 0));
    control_points_.push_back(vector(100, 100, 0));

    bezier_ = std::make_shared<continuous_path_bezier>(control_points_);
    bezier_path_ = bezier_->get_path();
    
    catmull_ = std::make_shared<continuous_path_catmull>(control_points_);
    catmull_path_ = catmull_->get_path();
}

void continuous_path_following_demo::update() {    
}

void continuous_path_following_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.6f, 0.0f, 0.0f);
    for(auto & v : bezier_path_) {
        //render_spot(v);
    }
    glColor3f(0.6f, 0.0f, 1.0f);
    for(auto & v : catmull_path_) {
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

