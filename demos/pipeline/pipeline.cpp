#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <array>

using namespace graphics_utils;
using namespace asiant;

namespace {
    constexpr int number_of_obstacles = 38;
}

class pipeline_demo : public application {
public:
    pipeline_demo();

    //virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    //virtual const char* get_title();
    //virtual void get_help_text(const char*** lines, unsigned *line_count);
    //virtual unsigned get_status_count();
    //virtual const char* get_status_text(unsigned slot);
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
    auto accel = (real)50.0;
    kinematic_ = std::make_shared<kinematic>();
    kinematic_->set_position(vector((real)5.0, (real)5.0, (real)1.0));
    kinematic_->set_orientation(random_real(pi));
    kinematic_->set_velocity(vector((real)0.0, (real)0.0, (real)0.0));
    kinematic_->set_rotation((real)0.0);

    wander_ = std::make_shared<wander>();
    wander_->set_max_acceleration(accel);
    wander_->set_character(kinematic_);
    wander_->set_max_rotation(1.0);
    wander_->set_max_speed(20.0);
    
    for(unsigned i = 0; i < number_of_obstacles; ++i) {
        spheres_[i] = std::make_shared<sphere>();
        spheres_[i]->center_ = vector(random_real((real)800.0), random_real((real)600.0), 0.0);
        spheres_[i]->radius_ = random_real((real)5.0, (real)20.0);
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

void pipeline_demo::update() {
    float duration = timer::get().last_frame_duration * 0.001f;
    auto steer = std::make_shared<steering>();

    steering_pipe_->get_steering(steer);

    // Update the kinematic
    kinematic_->integrate(*steer, (real)1.0, duration);
    kinematic_->update_to_face_velocity();

    // Check for maximum speed
    kinematic_->trim_max_speed((real)20.0);

	// Check for proximity to the goal, and create a new one if we are near.
	if (auto_new_goal && kinematic_->get_position().distance(targeter_->goal_.position_.value()) < 2.0f) {
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

}

void pipeline_demo::create_random_goal() {
    bool okay;
    do {
        okay = true;
        targeter_->goal_.position_ = vector(random_real(width_), random_real(height_), 0.0);
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

