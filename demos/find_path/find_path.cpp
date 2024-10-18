#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 
#include <algorithm>

using namespace asiant;
using namespace graphics_utils;

constexpr real square_size = 15;
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

class euclidean_heuristic : public heuristic {
public:
    euclidean_heuristic(int goal_node) : heuristic(goal_node) {} 
    real estimate(int node_value) {
        return (map_[node_value] - map_[goal_]).magnitude();
    }
    void set_location_map(std::vector<vector> & map) {
        map_ = map;
    }
private:
    std::vector<vector> map_;
};


class find_path_demo : public application {
public:
    find_path_demo();
    
    std::shared_ptr<level> level_;
    std::shared_ptr<graph> graph_;
    std::vector<std::shared_ptr<square>> squares_;
    std::vector<std::shared_ptr<vector>> path_;
    std::vector<std::shared_ptr<square>> path_squares_;
    int target_index_;
    seek seek_;

    virtual void update();
    virtual void display();
};


find_path_demo::find_path_demo() {

    // create the level, obtain graph and shortest path.
    level_ = std::make_shared<level>("test_levels/level_4.txt");
    graph_ = level_->get_graph();
    auto locations = level_->get_locations();
    std::vector<vector> node_to_location_map;

    for(auto & location : locations) {
        auto dv = std::div(location, level_->get_cols()); 
        auto j = dv.quot;
        auto i = dv.rem;
        auto x = (i + 0.5) * square_size + square_start_x;
        auto y = (j + 0.5) * square_size + square_start_y;
        node_to_location_map.push_back(vector(x, y, 0));
    }
    
    auto h = std::make_shared<euclidean_heuristic>(level_->get_end());
    h->set_location_map(node_to_location_map);

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
        }
    }

    auto path = a_star(*graph_, level_->get_begin(), level_->get_end(), h);
    for(auto & node : path) {
        auto dv = std::div(locations[node], level_->get_cols()); 
        auto j = dv.quot;
        auto i = dv.rem;

        auto ps = std::make_shared<square>();
        ps->x = i * square_size + square_start_x;
        ps->y = j * square_size + square_start_y;
        ps->on_solution_path = true;
        ps->is_walkable = true;
        path_squares_.push_back(ps);

        auto v = std::make_shared<vector>(ps->x + 0.5 * square_size,
                                          ps->y + 0.5 * square_size,
                                          0.0);
        path_.push_back(v);
    }

    // fill out the seek object
    auto character = std::make_shared<asiant::kinematic>();
    character->set_position(*path_[0]);
    character->set_velocity(asiant::vector(0, 0, 0));
    seek_.set_character(character);
    seek_.set_max_acceleration(10.0);
    target_index_ = 1;
    seek_.set_target(path_[target_index_]);
}

void find_path_demo::update() {
    float duration = asiant::timer::get().last_frame_duration * 0.005f;
    auto steer = std::make_shared<steering>();
    seek_.get_steering(steer);
    seek_.get_character()->integrate(*steer, (real)0.3, duration);
    seek_.get_character()->update_to_face_velocity();
    seek_.get_character()->trim_max_speed((real)10.0);
    auto delta = (*path_[target_index_] - seek_.get_character()->get_position()).magnitude(); 
    if(delta < 4) {
        target_index_++;
        if(target_index_ < path_.size())
            seek_.set_target(path_[target_index_]);
    }
    glutPostRedisplay();
}


void find_path_demo::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 0.0f);
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

