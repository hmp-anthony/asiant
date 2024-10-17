#include <asiant/asiant.hpp>

#include"../application.hpp"
#include"../graphics_utils.hpp"

#include <stdio.h>
#include <math.h> 

using namespace asiant;
using namespace graphics_utils;

constexpr real square_size = 10;
constexpr real square_start_x = 100;
constexpr real square_start_y = 100;

struct square {

};

class find_path_demo : public application {
public:
    find_path_demo();
    
    std::shared_ptr<level> level_;
    std::shared_ptr<graph> graph_;

    virtual void update();
    virtual void display();
};

find_path_demo::find_path_demo() {
    level_ = std::make_shared<level>("test_levels/level_2.txt");
    graph_ = level_->get_graph();
    auto path = dijkstra(*graph_, level_->get_begin(), level_->get_end());
}

void find_path_demo::update() {
}


void find_path_demo::display() {
}

application* get_application() {
    return new find_path_demo();
}

