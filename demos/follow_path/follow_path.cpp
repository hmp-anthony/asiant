#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 

using namespace graphics_utils;

class follow_path_demo : public application {
public:

    follow_path_demo();
    asiant::follow_path_seek seek_;
    virtual void display();
    virtual void update();

};

follow_path_demo::follow_path_demo() {
    auto character = std::make_shared<asiant::kinematic>();
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character->set_position(pos);
    character->set_velocity(vel);
    seek_.set_character(character);

    asiant::path p;
    asiant::line_segment l1(asiant::vector(100, 100, 0), asiant::vector(500, 100, 0));
    p.add_line_segment(l1);
    asiant::line_segment l2(asiant::vector(500, 100, 0), asiant::vector(700, 200, 0));
    p.add_line_segment(l2);
    asiant::line_segment l3(asiant::vector(700, 200, 0), asiant::vector(700, 500, 0));
    p.add_line_segment(l3);
    asiant::line_segment l4(asiant::vector(700, 500, 0), asiant::vector(100, 100, 0));
    p.add_line_segment(l4);

    asiant::path_constrained_entity pce;
    pce.set_path(p);
    seek_.set_path_constrained_entity(pce);
}

void follow_path_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    auto steer = std::make_shared<asiant::steering>();
    seek_.get_steering(steer);
    seek_.get_character()->update(steer, duration, 10.0);
    seek_.get_character()->update_to_face_velocity();
    application::update();
}

void follow_path_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render_agent(seek_.get_character());
    render_agent(seek_.get_target());
}

application* get_application() {
    return new follow_path_demo();
}

