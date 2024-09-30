#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 

using namespace graphics_utils;

class seek_demo : public application {
public:

    seek_demo();
    asiant::seek seek_;
    virtual void display();
    virtual void update();

};

seek_demo::seek_demo() {
    auto character = std::make_shared<asiant::kinematic>();
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character->set_position(pos);
    character->set_velocity(vel);
    seek_.set_character(character);

    auto target = std::make_shared<asiant::vector>(700, 500, 0);
    seek_.set_target(target);
}

void seek_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    auto steer = std::make_shared<asiant::steering>();
    seek_.get_steering(steer);
    seek_.get_character()->update(steer, duration, 10.0);
    seek_.get_character()->update_to_face_velocity();
    application::update();
}

void seek_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render_agent(seek_.get_character());
    render_agent(seek_.get_target());
}

application* get_application() {
    return new seek_demo();
}

