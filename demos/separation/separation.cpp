#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

using asiant::real;

void circle(real x, real y, real r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n < 2*segments + 1; ++n) {
        real const t = asiant::pi * (real)n / (real)segments;
        glVertex2f(x + sin(t) * r, y + cos(t) * r);
    }
    glEnd();
}


class separation_demo : public application {
public:
    struct separation {
        asiant::separation sep;

        void render() {
            auto character = sep.get_character();
            auto targets = sep.get_targets();

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
    
            for(auto & target:targets) {
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

        }
    };

    separation_demo();
    separation s;
    virtual void display();
    virtual void update();

};

separation_demo::separation_demo() {
    asiant::kinematic character;
    auto pos = asiant::vector(700, 500, 0);
    auto vel = asiant::vector(10.0, 1.0, 0);
    character.set_position(pos);
    character.set_velocity(vel);
    s.sep.set_character(character);

    std::vector<asiant::kinematic> targets;

    asiant::kinematic target1;
    pos = asiant::vector(500, 400, 0);
    vel = asiant::vector(0.0, 0.0, 0.0);
    target1.set_position(pos);
    target1.set_velocity(vel);
    targets.push_back(target1);

    asiant::kinematic target2;
    pos = asiant::vector(600, 200, 0);
    vel = asiant::vector(0.0, 0.0, 0.0);
    target2.set_position(pos);
    target2.set_velocity(vel);
    targets.push_back(target2);

    asiant::kinematic target3;
    pos = asiant::vector(200, 200, 0);
    vel = asiant::vector(0.0, 10.0, 0.0);
    target3.set_position(pos);
    target3.set_velocity(vel);
    targets.push_back(target3);

    asiant::kinematic target4;
    pos = asiant::vector(100, 100, 0);
    vel = asiant::vector(0.0, 0.0, 0.0);
    target4.set_position(pos);
    target4.set_velocity(vel);
    targets.push_back(target4);

    s.sep.set_targets(targets);

    s.sep.set_threshold(500.0);
    s.sep.set_decay_coefficient(750.0);
    s.sep.set_max_acceleration(25.0);
}

void separation_demo::update() {
    real duration = (real)asiant::timer::get().last_frame_duration * 0.01f;
    for(auto& target: s.sep.get_targets()) {
        target.update(duration);
    }
    s.sep.get_character().update(duration, 10);

    // Make the environment toroidial.
    auto pos = s.sep.get_character().get_position();
    if(pos[0] < 0.0) {
        pos[0] += width;
    }
    if(pos[0] > width) {
        pos[0] -= width;
    }
    if(pos[1] < 0.0) {
        pos[1] += height;
    }
    if(pos[1] > height) {
        pos[1] -= height;
    }
    s.sep.get_character().set_position(pos);

    pos = s.sep.get_targets()[2].get_position();
    if(pos[0] < 0.0) {
        pos[0] += width;
    }
    if(pos[0] > width) {
        pos[0] -= width;
    }
    if(pos[1] < 0.0) {
        pos[1] += height;
    }
    if(pos[1] > height) {
        pos[1] -= height;
    }
    s.sep.get_targets()[2].set_position(pos);

    s.sep.update();
    application::update();
}

void separation_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    s.render();
}

application* get_application() {
    return new separation_demo();
}

