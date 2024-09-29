#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"
#include "flock_steer.hpp"

#include <stdio.h>
#include <math.h> 

// This is the size of the world in both directions from 0 (i.e. from
// -WORLD_SIZE to +WORLD_SIZE)
#define WORLD_SIZE 50

// This is the gap between grid lines
#define GRID_SIZE 5

// The number of boids in the simulation
#define BOIDS 30

// The relative blend weights
#define SEPARATION_WEIGHT ((aicore::real)1.0)
#define COHESION_WEIGHT ((aicore::real)1.0)
#define VMA_WEIGHT ((aicore::real)2.0)


using asiant::kinematic;

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

real random_real(real max)
{
   return max * (real(rand()) / RAND_MAX);
}

class flocking_demo : public application {
public:
    flocking_demo();

    virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    virtual const char* get_title();
    virtual void get_help_text(const char*** lines, unsigned *lineCount);
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);

private:
    flock flock_;

    bool separation_on;
    bool cohesion_on;
    bool velocity_match_on;

    std::shared_ptr<separation> separation_;
    std::shared_ptr<cohesion> cohesion_;
    std::shared_ptr<velocity_match_and_align> separation_;
    std::shared_ptr<blended_steering> blended_steering_;
};

flocking_demo::flocking_demo() : application(), 
                                 separation_on(true),
                                 cohesion_on(true),
                                 velocity_match(true) {
    static const real accel = (real) 10.0;

    for(unsigned i = 0; i < BOIDS; ++i) {
        auto k = std::make_shared<kinematic>();

        auto position = vector(random_real(width), random_real(height), 0);
        k->set_position(position);

        auto orientation = random_real(asiant::pi);
        k->set_orientation(orientation);

        auto velocity = vector(random_real(1.0), random_real(1.0), 0);
        k->set_velocity(velocity);

        auto rotation = 0;
        k->set_rotation(rotation);

        flock_.boids.push_back(k);
    }


}

application* get_application()
{
    return new flocking_demo();
}
