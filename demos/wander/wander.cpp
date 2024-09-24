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


class wander_demo : public application {
public:
    struct wander_data {
        asiant::wander w;

        void render() {
            auto character = w.get_character();

            // let's draw the character
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
        }
    };

    wander_demo();
    wander_data wd;
    virtual void display();
    virtual void update();

};

wander_demo::wander_demo() {
    asiant::kinematic character;
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character.set_position(pos);
    character.set_velocity(vel);
    wd.w.set_character(character);

    wd.w.set_max_speed(3.0);
    wd.w.set_max_rotation(1.0);
}

void wander_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    wd.w.get_character().update(duration);

    // Make the environment toroidial.
    auto pos = wd.w.get_character().get_position();
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
    wd.w.get_character().set_position(pos);
    wd.w.update();
    application::update();
}

void wander_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    wd.render();
}

application* get_application() {
    return new wander_demo();
}

