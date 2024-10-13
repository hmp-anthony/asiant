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
    int path_location_;
    std::shared_ptr<seek> seek_;
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
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(catmull_path_[200]);
}

#include<iostream>
void continuous_path_following_demo::update() {    

}

void continuous_path_following_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.6f, 1.0f, 0.0f);
    render_agent(kinematic_);

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

