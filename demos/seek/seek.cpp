#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 
#include <iostream>

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < segments + 1; ++n) {
        float const t = 2 * 3.14159 * (float)n / (float)segments;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}


class seek_demo : public application {
public:
    struct seek {
        asiant::seek_with_max_speed seek_max_speed;

        void render() {
            auto character = seek_max_speed.get_character();
            auto target = seek_max_speed.get_target();

            // let's draw the character first
            auto pos = character.get_position();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 0);
            circle(0.0, 0.0, 20.0, 3);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();

            pos = target.get_position();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(0, 0, 1);
            circle(0.0, 0.0, 20.0, 3);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();

        }
    };

    seek_demo();
    seek sk;
    virtual void display();
    virtual void update();

};

seek_demo::seek_demo() {
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(1.0, -2.0, 0);
    c[0].kin.set_position(pos);
    c[0].kin.set_velocity(vel);
    auto acc = asiant::vector(0.01, 0.001, 0.0);
    asiant::real ang_acc = 0.0;
    c[0].s.set_linear(acc);
    c[0].s.set_angular(ang_acc);
}

void seek_demo::update() {
    sk.seek_max_speed.update();
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

