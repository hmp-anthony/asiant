#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <cstdlib>

class application {
public:
    application();
    virtual const char* get_title();
    virtual void init_graphics();
    virtual void set_view();
    virtual void deinit();
    virtual void display();
    virtual void update();
    virtual bool key(unsigned char key);
    virtual void resize(int width, int height);
    virtual void mouse(int button, int state, int x, int y);
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);
    void display_help();
protected:
    int height_;
    int width_;
    bool help_;
};

#endif
