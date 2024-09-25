#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

/* here we compare a seek to a pursue */

using asiant::pi;

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
            auto seek_character = swms.set_character();
            auto target = swms.get_target();

            // let's draw the pursue character first
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

            // let's draw the seek character
            pos = pursue_character.get_position();
            ori = pursue_character.get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 1, 0);
            circle(0.0, 0.0, 20.0, 9);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();
    
            // draw target
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

seek_demo::seek_demo() {
    asiant::kinematic pursue_character;
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(3.0, 7.0, 0);
    pursue_character.set_position(pos);
    pursue_character.set_velocity(vel);
    pvsd.prsu.set_character(pursue_character);


    asiant::kinematic target;
    pos = asiant::vector(700, 500, 0);
    vel = asiant::vector(-5.0, -4.0, 0.0);
    target.set_position(pos);
    target.set_velocity(vel);
    sk.prsu.set_target(target);

    sk.prsu.set_max_speed(6.0);
    sk.prsu.set_radius(100.0);
    sk.prsu.set_time_to_target(0.1);
}

void seek_demo::update() {
    real duration = (real)asiant::timer::get().last_frame_duration * 0.01f;
    sk.prsu.get_target().update(duration);
    sk.prsu.get_character().update(duration);
    sk.prsu.update();
    application::update();
}

void seek_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    sk.render();
}

application* get_application() {
    return new seek_demo();
}

