#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

/* here we compare a seek to a pursue */

using asiant::pi;
using asiant::real;

void circle(real x, real y, real r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < segments + 1; ++n) {
        real const t = 2 * pi * (real)n / (real)segments;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}


class pursue_demo : public application {
public:
    struct pursue_vs_seek_data {
        asiant::pursue prsu;
        asiant::seek_with_max_speed swms;

        void render() {
            auto pursue_character = prsu.get_character();
            auto seek_character = swms.get_character();
            auto target = swms.get_target();

            // let's draw the pursue character first - RED
            auto pos = pursue_character.get_position();
            auto ori = pursue_character.get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 0);
            circle(0.0, 0.0, 20.0, 9);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();

            // let's draw the seek character - PINK
            pos = seek_character.get_position();
            ori = seek_character.get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 1);
            circle(0.0, 0.0, 20.0, 9);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();
    
            // draw target - BLUE
            pos = target.get_position();
            ori = target.get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(0, 0, 1);
            circle(0.0, 0.0, 20.0, 9);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();

        }
    };

    pursue_demo();
    pursue_vs_seek_data pvsd;
    virtual void display();
    virtual void update();

};

pursue_demo::pursue_demo() {
    asiant::kinematic pursue_character;
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(3.0, 7.0, 0);
    pursue_character.set_position(pos);
    pursue_character.set_velocity(vel);
    pvsd.prsu.set_character(pursue_character);

    asiant::kinematic seek_character;
    pos = asiant::vector(300, 300, 0);
    vel = asiant::vector(3.0, 7.0, 0);
    seek_character.set_position(pos);
    seek_character.set_velocity(vel);
    pvsd.swms.set_character(seek_character);

    asiant::kinematic target;
    pos = asiant::vector(750, 550, 0);
    vel = asiant::vector(-5.0, -9.0, 0.0);
    target.set_position(pos);
    target.set_velocity(vel);
    pvsd.prsu.set_target(target);
    pvsd.swms.set_target(target);

    pvsd.swms.set_max_speed(9.0);
    pvsd.prsu.set_max_speed(9.0);
    pvsd.prsu.set_max_prediction(0.05);
}

void pursue_demo::update() {
    real duration = (real)asiant::timer::get().last_frame_duration * 0.01f;
    pvsd.prsu.get_target().update(duration);
    pvsd.prsu.get_character().update(duration);
    pvsd.swms.get_target().update(duration);
    pvsd.swms.get_character().update(duration);
    pvsd.prsu.update();
    pvsd.swms.update();
    application::update();
}

void pursue_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    pvsd.render();
}

application* get_application() {
    return new pursue_demo();
}

