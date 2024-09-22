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
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 0);
            circle(0.0, 0.0, 20.0, 3);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();

        }
    };

    basic_steering_demo();
    character c[3];
    virtual void display();
    virtual void update();

};

basic_steering_demo::basic_steering_demo() {
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(1.0, -2.0, 0);
    c[0].kin.set_position(pos);
    c[0].kin.set_velocity(vel);
    auto acc = asiant::vector(0.01, 0.001, 0.0);
    asiant::real ang_acc = 0.0;
    c[0].s.set_linear(acc);
    c[0].s.set_angular(ang_acc);

    pos = asiant::vector(500, 400, 0);
    vel = asiant::vector(-1.3, -2.5, 0);
    c[1].kin.set_position(pos);
    c[1].kin.set_velocity(vel);
    acc = asiant::vector(0.01, 0.06, 0.0);
    ang_acc = 0.0;
    c[1].s.set_linear(acc);
    c[1].s.set_angular(ang_acc);

    pos = asiant::vector(600, 100, 0);
    vel = asiant::vector(-1.1, -2.0, 0);
    c[2].kin.set_position(pos);
    c[2].kin.set_velocity(vel);
    acc = asiant::vector(0.01, 0.06, 0.0);
    ang_acc = 0.0;
    c[2].s.set_linear(acc);
    c[2].s.set_angular(ang_acc);
}

void basic_steering_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    c[0].kin.smooth_update_to_face_velocity(c[0].s, duration, 110);
    c[1].kin.smooth_update_to_face_velocity(c[1].s, duration, 100);
    c[2].kin.smooth_update_to_face_velocity(c[2].s, duration, 105);
    application::update();
}

void basic_steering_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    c[0].render();
    c[1].render();
    c[2].render();
}

application* get_application() {
    return new basic_steering_demo();
}

