#include<cstring>

#include "application.hpp"
#include "ogl_headers.hpp"

application::application() : help_(false) {}

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
    glOrtho(0.0, (double)width_, 0.0, (double)height_, -1.0, 1.0);
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
    return "asiant";
}

void application::deinit()
{
}

void application::update()
{
    glutPostRedisplay();
}

bool application::key(unsigned char key)
{
    switch(key) {
        case 'h':
            help_ = !help_;
            return true;
    }
    return false;
}


void application::resize(int width_, int height_)
{
    // Avoid the divide by zero.
    if (height_ <= 0) height_ = 1;

    // Set the internal variables and update the view
    application::width_ = width_;
    application::height_ = height_;
    glViewport(0, 0, width_, height_);
    set_view();
}

void application::mouse(int button, int state, int x, int y) {
}

static const char *default_help[] = {
    "No help_ is available."
};

void application::get_help_text(const char*** lines, unsigned* line_count) {
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

unsigned application::get_status_count() {
    return 0;
}

const char* application::get_status_text(unsigned slot) {
    return "none";
}

static const char *help_hidden[] = {
    "Press H for help."
};

void application::display_help()
{
    const char** help_text = help_hidden;
    unsigned lines = 1;

    // Retrieve the text for this application
    if (help_) {
        get_help_text(&help_text, &lines);
    }

    // Set up 2D mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width_, 0, height_, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render the text
    glColor3f(0,0,0);
    for (unsigned i = 0; i < lines; i++)
    {
        glRasterPos2i(10, height_ - 20 - i*15);

        const char *c = help_text[i];
        while (*c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
            c++;
        }
    }

    // Render the status slots (always done)
    unsigned slots = get_status_count();
    for (unsigned i = 0; i < slots; i++)
    {
        const char *text = get_status_text(i);
        int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_10,
                                         (const unsigned char*)text);

        // Right align the string
        glRasterPos2i(width_-10-textWidth, height_-20-i*15);

        // And render
        const char *c = text;
        while (*c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
            c++;
        }
    }


    // Break down 2D mode
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
