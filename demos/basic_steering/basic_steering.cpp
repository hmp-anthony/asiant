#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>
#include <math.h> 

class basic_steering_demo : public application {
public:
    basic_steering_demo();
    virtual void display();
};

basic_steering_demo::basic_steering_demo() {}

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n <= segments; ++n ) {
            float const t = 2 * 3.14159 * (float)n / (float)segments;
            glVertex2f(x + sin(t) * r, y + cos(t) * r);
        }
    glEnd();
}

void basic_steering_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 0, 0);
    circle(300.0, 300.0, 100.0, 20);

}

application* get_application() {
    return new basic_steering_demo();
}

