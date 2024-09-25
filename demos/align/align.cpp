#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

using asiant::pi;

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < segments + 1; ++n) {
        asiant::real const t = 2 * pi * (asiant::real)n / (asiant::real)segments;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}


class align_demo : public application {
public:
    struct align_demo_struct {
        asiant::align agn;

        void render() {
            auto character = agn.get_character();
            auto target = agn.get_target();

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

    align_demo();
    align_demo_struct agns;
    virtual void display();
    virtual void update();

};

align_demo::align_demo() {
    asiant::kinematic character;
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0, 0.0, 0);
    character.set_position(pos);
    character.set_velocity(vel);
    agns.agn.set_character(character);

    asiant::kinematic target;
    pos = asiant::vector(400, 300, 0);
    vel = asiant::vector(0.0, 0.0, 0.0);
    asiant::real rot = 0.5; 
    target.set_position(pos);
    target.set_velocity(vel);
    target.set_rotation(rot);
    agns.agn.set_target(target);

    agns.agn.set_max_angular_acceleration(0.1);
    agns.agn.set_max_rotation(1.0);
    agns.agn.set_target_radius(0.5);
    agns.agn.set_slow_radius(1.0);
    agns.agn.set_time_to_target(1.0);
}

void align_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    agns.agn.get_target().update(duration);
    agns.agn.get_character().update(duration);
    agns.agn.update();
    application::update();
}

void align_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    agns.render();
}

application* get_application() {
    return new align_demo();
}

