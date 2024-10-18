#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <algorithm>

using namespace asiant;
using namespace graphics_utils;

constexpr real square_size = 20;
constexpr real square_start_x = 100;
constexpr real square_start_y = 20;

struct square {
    // these coordinates are the bottom left vertex of the square.
    real x;
    real y;
    bool is_walkable;
    bool on_solution_path;

    void render() {
        if(is_walkable && on_solution_path) {
            glColor3f(1.0f, 0.0f, 0.0f);
        } else if(is_walkable || on_solution_path) {
            glColor3f(0.0f, 1.0f, 0.0f);
        } else {
            glColor3f(0.0f, 0.0f, 1.0f);
        }
        glBegin(GL_QUADS);
        glVertex2f(x,               y              );
        glVertex2f(x + square_size, y              );
        glVertex2f(x + square_size, y + square_size);
        glVertex2f(x,               y + square_size);
        glEnd();
    }
};

class find_path_demo : public application {
public:
    find_path_demo();
    
    std::shared_ptr<level> level_;
    std::shared_ptr<graph> graph_;
    std::vector<std::shared_ptr<square>> squares_;
    std::vector<int> path_;
    std::vector<std::shared_ptr<square>> path_squares_;
    std::vector<std::shared_ptr<vector>> targets_;
    int target_index_;
    seek seek_;

    virtual void update();
    virtual void display();
};

find_path_demo::find_path_demo() {

    // create the level, obtain graph and shortest path.
    level_ = std::make_shared<level>("test_levels/level_3.txt");
    graph_ = level_->get_graph();
    path_ = dijkstra(*graph_, level_->get_begin(), level_->get_end());
    auto locations = level_->get_locations();
    auto index_to_node_map = level_->get_index_to_node_map();
 
    // fill the squares container
    for(int j = 0; j < level_->get_rows(); ++j) {
        for(int i = 0; i < level_->get_cols(); ++i) {
            auto s = std::make_shared<square>();
            auto index = j * level_->get_cols() + i; 
            if(std::find(locations.begin(), locations.end() , index) != locations.end()) {
                s->is_walkable = true;
            } else {
                s->is_walkable = false;
            }
            s->on_solution_path = false;
            s->x = i * square_size + square_start_x;
            s->y = j * square_size + square_start_y;
            squares_.push_back(s);

            auto ps = std::make_shared<square>();
            auto node = index_to_node_map[index];

            if(std::find(path_.begin(), path_.end() , node) != path_.end()) {
                ps->x = i * square_size + square_start_x;
                ps->y = j * square_size + square_start_y;
                auto v = std::make_shared<vector>(ps->x + 0.5 * square_size, 
                                                  ps->y + 0.5 * square_size, 
                                                  0.0);
                targets_.push_back(v);
                ps->is_walkable = true;
                ps->on_solution_path = true;
                path_squares_.push_back(ps);
            }

        }
    }

    // fill out the seek object
    auto character = std::make_shared<asiant::kinematic>();
    character->set_position(asiant::vector((*targets_[0])[0],(*targets_[0])[1], 0));
    character->set_velocity(asiant::vector(0, 0, 0));
    seek_.set_character(character);
    seek_.set_max_acceleration(10.0);
    target_index_ = 1;
    seek_.set_target(targets_[target_index_]);
}

void find_path_demo::update() {
    float duration = asiant::timer::get().last_frame_duration * 0.005f;
    auto steer = std::make_shared<steering>();
    seek_.get_steering(steer);
    seek_.get_character()->integrate(*steer, (real)0.5, duration);
    seek_.get_character()->update_to_face_velocity();
    seek_.get_character()->trim_max_speed((real)10.0);
    auto delta = (*targets_[target_index_] - seek_.get_character()->get_position()).magnitude(); 
    if(delta < 1) {
        target_index_++;
        if(target_index_ < targets_.size())
            seek_.set_target(targets_[target_index_]);
    }
    glutPostRedisplay();
}


void find_path_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render_agent(seek_.get_character());
    
    glColor3f(0.0f, 0.0f, 0.0f);
    render_spot(seek_.get_target());
    for(auto & s : path_squares_) {
        s->render();
    }
    for(auto & s : squares_) {
        s->render();
    }
}

application* get_application() {
    return new find_path_demo();
}

