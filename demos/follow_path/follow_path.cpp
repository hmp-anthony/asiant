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


class follow_path_demo : public application {
public:
    struct follow {
        asiant::follow_path fp;;

        void render() {
            auto character = fp.get_character();
            auto target = fp.get_target();

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
            circle(0.0, 0.0, 5.0, 6);
            glPopMatrix();           

        }
    };

    follow_path_demo();
    follow f;
    virtual void display();
    virtual void update();

};

follow_path_demo::follow_path_demo() {
    asiant::kinematic character;
    auto pos = asiant::vector(100, 100, 0);
    auto vel = asiant::vector(0.0, 0.0, 0);
    character.set_position(pos);
    character.set_velocity(vel);
    f.fp.set_character(character);

    asiant::path p;
    asiant::line_segment l1(asiant::vector(100, 100, 0), asiant::vector(600, 100, 0));
    p.add_line_segment(l1);
    asiant::line_segment l2(asiant::vector(600, 100, 0), asiant::vector(600, 500, 0));
    p.add_line_segment(l2);
    asiant::line_segment l3(asiant::vector(600, 500, 0), asiant::vector(100, 500, 0));
    p.add_line_segment(l3);
    asiant::line_segment l4(asiant::vector(100, 500, 0), asiant::vector(100, 100, 0));
    p.add_line_segment(l4);

    f.fp.set_path(p);
    f.fp.set_max_speed(10.0);
    f.fp.set_path_offset(0.1);
}

void follow_path_demo::update() {
    float duration = (float)asiant::timer::get().last_frame_duration * 0.01f;
    f.fp.get_target().update(duration);
    f.fp.get_character().update(duration);
    f.fp.update();
    application::update();
}

void follow_path_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    f.render();
}

application* get_application() {
    return new follow_path_demo();
}

