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
    asiant::real path_velocity_;

    virtual void display();
    virtual void update();
    virtual bool key(unsigned char key);
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual const char* get_title();
    void print_velocity();
};

follow_path_demo::follow_path_demo() {
    path_velocity_ = 35.0;

    auto character = std::make_shared<asiant::kinematic>();
    character->set_position(asiant::vector(300, 300, 0));
    character->set_velocity(asiant::vector(0, 0, 0));
    seek_.set_character(character);
    
    seek_.set_max_acceleration(20.0);
    seek_.set_target_radius(50.0);
    seek_.set_velocity_radius(100.0);
    seek_.set_time_to_target(1.0);
    seek_.set_max_speed(50.0);

    asiant::path p;
    asiant::line_segment l1(asiant::vector(100, 100, 0), asiant::vector(500, 100, 0));
    p.add_line_segment(l1);
    asiant::line_segment l2(asiant::vector(500, 100, 0), asiant::vector(700, 200, 0));
    p.add_line_segment(l2);
    asiant::line_segment l3(asiant::vector(700, 200, 0), asiant::vector(700, 500, 0));
    p.add_line_segment(l3);
    asiant::line_segment l4(asiant::vector(700, 500, 0), asiant::vector(100, 100, 0));
    p.add_line_segment(l4);

    auto pce = std::make_shared<asiant::path_constrained_entity>();
    pce->set_path(p);
    seek_.set_path_constrained_entity(pce);
}

void follow_path_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    auto steer = std::make_shared<asiant::steering>();
    seek_.get_steering(steer);
    seek_.get_character()->update(steer, duration, 20.0);
    seek_.get_character()->update_to_face_velocity();
    seek_.get_path_constrained_entity()->update(duration, path_velocity_);
    application::update();
}

static const char *default_help[] = {
    "asiant: follow path demo",
    "H - Toggle help.",
    "",
    "Toggle behaviour element:",
    "p - increase velocity",
    "m - decrease velocity",
};

void follow_path_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.6f, 0.0f, 0.6f);
    render_agent(seek_.get_character());
    render_agent(seek_.get_target());
    display_help();
    print_velocity();
}

bool follow_path_demo::key(unsigned char key) {
    if(application::key(key)) return true;
    switch(key) {
        case 'p':
            path_velocity_ += 5.0;
            return true;
        case 'm':
            path_velocity_ -= 5.0;
            if(path_velocity_ < 10.0) { path_velocity_ = 10.0; }
            return true;
    }

    return false;
}

void follow_path_demo::get_help_text(const char*** lines, unsigned *line_count) {
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

const char* follow_path_demo::get_title()
{
    return default_help[0];
}

void follow_path_demo::print_velocity()
{
    const char *text = std::to_string(path_velocity_).c_str();
    int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                         (const unsigned char*)text);
    // Right align the string
    glRasterPos2i(width_-10-textWidth, height_-20);

    // And render
    const char *c = text;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        c++;
    }
}

application* get_application() {
    return new follow_path_demo();
}

