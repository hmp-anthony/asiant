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
    constexpr int number_of_obstacles = 10;
}

class priority_demo : public application {
public:
    priority_demo();

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
    std::array<std::shared_ptr<avoid_sphere>, number_of_obstacles> avoids_;
    std::shared_ptr<wander> wander_;
    std::shared_ptr<blended_steering> blended_steering_;
    std::shared_ptr<priority_steering> priority_steering_;
    bool is_blended; 
};

priority_demo::priority_demo() : application(), is_blended(false) {
    static const real accel = (real) 50.0;
}

application* get_application() {
    return new priority_demo();
}

