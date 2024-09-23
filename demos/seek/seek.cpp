#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

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
        asiant::seek_with_satisfaction_radius seek_satisfaction_radius;

        void render() {
            auto character = seek_satisfaction_radius.get_character();
            auto target = seek_satisfaction_radius.get_target();

            // let's draw the character first
            auto pos = character.get_position();
            auto ori = character.get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 0);
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

    seek_demo();
    seek sk;
    virtual void display();
    virtual void update();

};

seek_demo::seek_demo() {
    asiant::kinematic character;
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(3.0, 7.0, 0);
    character.set_position(pos);
    character.set_velocity(vel);
    sk.seek_satisfaction_radius.set_character(character);

    asiant::kinematic target;
    pos = asiant::vector(700, 500, 0);
    vel = asiant::vector(-5.0, -4.0, 0.0);
    target.set_position(pos);
    target.set_velocity(vel);
    sk.seek_satisfaction_radius.set_target(target);

    sk.seek_satisfaction_radius.set_max_speed(6.0);
    sk.seek_satisfaction_radius.set_radius(100.0);
    sk.seek_satisfaction_radius.set_time_to_target(0.1);
}

void seek_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    sk.seek_satisfaction_radius.get_target().update(duration);
    sk.seek_satisfaction_radius.get_character().update(duration);
    sk.seek_satisfaction_radius.update();
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

