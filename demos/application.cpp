#include<cstring>

#include "application.hpp"
#include "ogl_headers.hpp"


void application::init_graphics()
{
    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    set_view();
}

void application::set_view()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void application::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(639, 319);
    glEnd();
}

const char* application::get_title()
{
    return "Cyclone Demo";
}

void application::deinit()
{
}

void application::update()
{
    glutPostRedisplay();
}

void application::key(unsigned char key)
{
}


void application::resize(int width, int height)
{
    // Avoid the divide by zero.
    if (height <= 0) height = 1;

    // Set the internal variables and update the view
    application::width = width;
    application::height = height;
    glViewport(0, 0, width, height);
    set_view();
}

void application::mouse(int button, int state, int x, int y)
{
}

void application::mouse_drag(int x, int y)
{
}

void application::render_text(float x, float y, const char *text, void *font)
{
    glDisable(GL_DEPTH_TEST);

    // Temporarily set up the view in orthographic projection.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

    // Move to modelview mode.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Ensure we have a font
    if (font == NULL) {
        font = GLUT_BITMAP_HELVETICA_10;
    }

    // Loop through characters displaying them.
    size_t len = strlen(text);

    glRasterPos2f(x, y);
    for (const char *letter = text; letter < text+len; letter++) {

        // If we meet a newline, then move down by the line-height
        // TODO: Make the line-height a function of the font
        if (*letter == '\n') {
            y -= 12.0f;
            glRasterPos2f(x, y);
        }
        glutBitmapCharacter(font, *letter);
    }

    // Pop the matrices to return to how we were before.
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}
