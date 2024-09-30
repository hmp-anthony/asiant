#include <asiant/asiant.hpp>

#include "ogl_headers.hpp"

namespace graphics_utils {
 
    void circle(float x, float y, float r, int segments) {
        glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n < segments + 1; ++n) {
            float const t = 2 * 3.14159 * (float)n / (float)segments;
            glVertex2f(x + sin(t) * r, y + cos(t) * r);
        }
        glEnd();
    }   

    void render_agent(std::shared_ptr<asiant::kinematic> k) {
        auto pos = k->get_position();
        auto ori = k->get_orientation();
        glPushMatrix();
        glTranslatef(pos[0], pos[1], 0);
        glRotatef(ori * 180 / 3.14159, 0, 0, 1);
        glColor3f(1, 0, 0); 
        circle(0.0, 0.0, 20.0, 9);
        glColor3f(0, 1, 0);
        circle(0.0, 20.0, 5.0, 20);
        glPopMatrix();
    } 

    void render_agent(std::shared_ptr<const asiant::vector> pos) {
        glPushMatrix();
        glTranslatef((*pos)[0], (*pos)[1], 0);
        glColor3f(0, 1, 0);
        circle(0.0, 0.0, 5.0, 20);
        glPopMatrix();
    } 
}

