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
    struct seek_data {
        asiant::seek sk;

        void render() {
            auto character = sk.get_character();
            auto target = sk.get_target();

            // let's draw the character first
            auto pos = character->get_position();
            auto ori = character->get_orientation();
            glPushMatrix();
            glTranslatef(pos[0], pos[1], 0);
            glRotatef(ori * 180 / 3.14159, 0, 0, 1);
            glColor3f(1, 0, 0);
            circle(0.0, 0.0, 20.0, 9);
            glColor3f(0, 1, 0);
            circle(0.0, 20.0, 5.0, 20);
            glPopMatrix();
    
            // draw target
            glPushMatrix();
            glTranslatef((*target)[0], (*target)[1], 0);
            glColor3f(0, 1, 0);
            circle(0.0, 0.0, 5.0, 20);
            glPopMatrix();

        }
    };

    seek_demo();
    seek_data skd;
    std::shared_ptr<asiant::kinematic> character;
    std::shared_ptr<asiant::vector> target;
    virtual void display();
    virtual void update();

};

seek_demo::seek_demo() {
    character = std::make_shared<asiant::kinematic>();
    auto pos = asiant::vector(300, 300, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character->set_position(pos);
    character->set_velocity(vel);
    skd.sk.set_character(character);

    target = std::make_shared<asiant::vector>(700, 500, 0);
    skd.sk.set_target(target);
}

void seek_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    auto s = skd.sk.get_steering();
    auto l = s->get_linear();
    character->update(s, duration, 10.0);
    character->update_to_face_velocity();
    application::update();
}

void seek_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    skd.render();
}

application* get_application() {
    return new seek_demo();
}

