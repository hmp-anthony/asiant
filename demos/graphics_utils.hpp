#include <asiant/asiant.hpp>

#include "ogl_headers.hpp"

namespace graphics_utils {
 
    void circle(asiant::real x, asiant::real y, asiant::real r, int segments) {
        glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n < segments + 1; ++n) {
            asiant::real const t = 2 * asiant::pi * (asiant::real)n / (asiant::real)segments;
            glVertex2f(x + sin(t) * r, y + cos(t) * r);
        }
        glEnd();
    }   

    void render_agent(std::shared_ptr<asiant::kinematic> k) {
        auto pos = k->get_position();
        auto ori = k->get_orientation();
        glPushMatrix();
        glTranslatef(pos[0], pos[1], 0);
        glRotatef(ori * 180.0 / asiant::pi, 0, 0, 1);
        circle(0.0, 0.0, 10.0, 9);
        glColor3f(0, 1, 0);
        circle(0.0, 12.0, 4.0, 9);
        glPopMatrix();
    } 

    void render_agent(std::shared_ptr<const asiant::vector> pos) {
        glPushMatrix();
        glTranslatef((*pos)[0], (*pos)[1], 0);
        glColor3f(0, 1, 0);
        circle(0.0, 0.0, 5.0, 9);
        glPopMatrix();
    } 
}

