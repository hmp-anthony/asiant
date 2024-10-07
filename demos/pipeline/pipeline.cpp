#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <array>

using namespace graphics_utils;
using namespace asiant;

namespace {
    constexpr int number_of_obstacles = 200;
}

class pipeline_demo : public application {
public:
    pipeline_demo();

    virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    virtual const char* get_title();
    virtual void get_help_text(const char*** lines, unsigned *line_count);
    virtual unsigned get_status_count();
    virtual const char* get_status_text(unsigned slot);
    virtual void create_random_goal();
private:
    std::shared_ptr<kinematic> kinematic_;
    std::array<std::shared_ptr<sphere>, number_of_obstacles> spheres_;

    std::shared_ptr<steering_pipe> steering_pipe_;
    std::shared_ptr<fixed_goal_targeter> targeter_;
    std::shared_ptr<avoid_spheres_constraint> constraint_;
    std::shared_ptr<basic_actuator> actuator_;

    std::shared_ptr<wander> wander_;
    bool auto_new_goal; 
};

pipeline_demo::pipeline_demo() : application(), auto_new_goal(true) {
    width_ = 800;
    height_ = 600;

    auto accel = (real)50.0;
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(vector((real)width_, (real)height_, (real)1.0));
    kinematic_->set_orientation(random_real(pi));
    kinematic_->set_velocity(vector((real)0.0, (real)20.0, (real)0.0));
    kinematic_->set_rotation((real)0.0);

    wander_ = std::make_shared<wander>();
    wander_->set_max_acceleration(accel);
    wander_->set_character(kinematic_);
    wander_->set_max_rotation(1.0);
    wander_->set_max_speed(50.0);
    
    for(unsigned i = 0; i < number_of_obstacles; ++i) {
        spheres_[i] = std::make_shared<sphere>();
        spheres_[i]->center_ = vector(random_real((real)width_), random_real((real)height_), 0.0);
        spheres_[i]->radius_ = random_real((real)10.0, (real)20.0);
    }
    
    steering_pipe_ = std::make_shared<steering_pipe>();
    steering_pipe_->set_character(kinematic_);

    targeter_ = std::make_shared<fixed_goal_targeter>();
    steering_pipe_->targeters_.push_back(targeter_);

    constraint_ = std::make_shared<avoid_spheres_constraint>();
    for(unsigned i = 0; i < number_of_obstacles; ++i) {
        constraint_->spheres_.push_back(spheres_[i]);
    }
    constraint_->avoid_margin_ = (real)2.0;
    steering_pipe_->constraints_.push_back(constraint_);

    actuator_ = std::make_shared<basic_actuator>();
    actuator_->max_acceleration_ = accel;
    steering_pipe_->set_actuator(actuator_);

    steering_pipe_->fallback_ = wander_;

    steering_pipe_->register_components();

    create_random_goal();
}

static const char *default_help[] = {
    "asiant: Steering Pipeline Demo",
    "H - Toggle help.",
	"",
	"A - Automatically move the goal",
	"N - Choose a new goal"
};

bool pipeline_demo::key(unsigned char key) {
    if (application::key(key)) return true;

    switch(key) {
	case 'a': case 'A':
		auto_new_goal = !auto_new_goal;
		return true;

	case 'n': case 'N':
		create_random_goal();
		return true;
    }

    return false;
}

void pipeline_demo::update() {
    float duration = timer::get().last_frame_duration * 0.005f;
    auto steer = std::make_shared<steering>();

    steering_pipe_->get_steering(steer);

    // Update the kinematic
    kinematic_->integrate(*steer, (real)1.0, duration);
    kinematic_->update_to_face_velocity();

    // Check for maximum speed
    kinematic_->trim_max_speed((real)20.0);
    kinematic_->set_position(trim_world(kinematic_->get_position(), width_, height_));

	// Check for proximity to the goal, and create a new one if we are near.
    auto distance = kinematic_->get_position().distance(targeter_->goal_.position_.value());
	if (auto_new_goal && distance < 5.0f) {
		create_random_goal();
	}

    glutPostRedisplay();
}

void pipeline_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the characters.
    glColor3f(0.0f, 0.3f, 0.6f);
    render_agent(kinematic_);

	// Draw the current goal
	glColor3f(0.6f, 0.0f, 0.0f);
	render_spot(targeter_->goal_.position_.value());

	// Draw the path
	if (steering_pipe_->path_)
	{
		auto path = steering_pipe_->path_;
		glColor3f(0.0f, 0.6f, 0.0f);
		glBegin(GL_LINES);
		auto v = path->goal_.position_.value();
	    auto u = path->character_->get_position();
        glVertex3f(v[0], v[1], v[2]);
        glVertex3f(u[0], u[1], u[2]);
        glEnd();
	}
    // Draw the obstacles
    glColor3f(0.4f, 0.4f, 0.4f);
    for (std::size_t i = 0; i < number_of_obstacles; ++i) {
		auto s = spheres_[i];
        glPushMatrix();
        circle(s->center_[0], s->center_[1], s->radius_, 20);
        glPopMatrix();
	}

    display_help();
}

const char* pipeline_demo::get_title()
{
    return default_help[0];
}

void pipeline_demo::get_help_text(const char*** lines,
                                  unsigned *line_count)
{
    *lines = default_help;
    *line_count = sizeof(default_help) / sizeof(const char*);
}

unsigned pipeline_demo::get_status_count()
{
    return 1;
}

const char* pipeline_demo::get_status_text(unsigned slot)
{
    
    bool is_goal = false;
    if(steering_pipe_->path_ && targeter_->goal_.position_) {
        is_goal = 
            steering_pipe_->path_->goal_.position_.value() == targeter_->goal_.position_.value();
    }
	if (constraint_->suggestion_used_) 
	{
		return "Honouring Constraint";
	}
	else if (is_goal)
	{
		return "Heading for goal";
	}
	else
	{
		return "Wandering";
	}
    
}

void pipeline_demo::create_random_goal() {
    bool okay;
    do {
        okay = true;
        targeter_->goal_.position_ = vector(random_real(width_), random_real(height_), 0.0);
        auto v = targeter_->goal_.position_.value();
        for(auto &  sphere : spheres_) {
            auto distance = (targeter_->goal_.position_.value() - sphere->center_).magnitude();
            if(distance < sphere->radius_ + 2.0f) {
                okay = false;
                break;
            }
        }
    } while(!okay);
}

application* get_application() {
    return new pipeline_demo();
}

