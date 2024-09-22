#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 
#include <iostream>

void draw_character(float x, float y, float r)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < 3; ++n ) {
        float const t = 2 * 3.14159 * (float)n / 3.0;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}


class basic_steering_demo : public application {
public:
    struct character {
        asiant::kinematic kin;
        asiant::steering s;

        void render() {
            asiant::vector pos = kin.get_position();
            asiant::real ori = kin.get_orientation();

            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori, 0, 0, 1);
            draw_character(0.0, 0.0, 20.0);
            glPopMatrix();

        }
    };

    basic_steering_demo();
    character c;
    virtual void display();
    virtual void update();

};

basic_steering_demo::basic_steering_demo() {
    auto pos = asiant::vector(300, 300, 0);
    c.kin.set_position(pos);
    auto acc = asiant::vector(0.1, 0.04, 0.0);
    asiant::real ang_acc = 0.4;
    c.s.set_linear(acc);
    c.s.set_angular(ang_acc);
}

void basic_steering_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    c.kin.update(c.s, duration);
    application::update();
}

void basic_steering_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 0, 0);
    c.render();
}

application* get_application() {
    return new basic_steering_demo();
}

