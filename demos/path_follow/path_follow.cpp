#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 

using namespace graphics_utils;

class path_follow_demo : public application {
public:

    path_follow_demo();
    asiant::follow_path_seek seek_;
    virtual void display();
    virtual void update();

};

path_follow_demo::path_follow_demo() {
    auto character = std::make_shared<asiant::kinematic>();
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character->set_position(pos);
    character->set_velocity(vel);
    seek_.set_character(character);

    asiant::path p;
    asiant::line_segment l1(asiant::vector(100, 100, 0), asiant::vector(700, 100, 0));
    p.add_line_segment(l1);
    asiant::line_segment l2(asiant::vector(700, 100, 0), asiant::vector(700, 500, 0));
    p.add_line_segment(l2);
    asiant::line_segment l3(asiant::vector(700, 500, 0), asiant::vector(100, 500, 0));
    p.add_line_segment(l3);
    asiant::line_segment l4(asiant::vector(100, 500, 0), asiant::vector(100, 100, 0));
    p.add_line_segment(l4);

    asiant::path_constrained_entity pce;
    pce.set_path(p);
    seek_.set_path_constrained_entity(pce);
}

void path_follow_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    auto steer = std::make_shared<asiant::steering>();
    seek_.get_steering(steer);
    seek_.get_character()->update(steer, duration, 20.0);
    seek_.get_character()->update_to_face_velocity();
    application::update();
}

void path_follow_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render_agent(seek_.get_character());
    render_agent(seek_.get_target());
}

application* get_application() {
    return new path_follow_demo();
}

