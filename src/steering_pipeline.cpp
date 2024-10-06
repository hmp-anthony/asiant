#include <asiant/steering_pipeline.hpp>

namespace asiant {

    goal::goal() {
        clear();
    }
    
    void goal::clear() {
        position_.reset();
        orientation_.reset();
        velocity_.reset();
        rotatioin_.reset();
    }

    void goal::update_goal(const goal& g) {
        if(g.position_) position_ = g.position_;
        if(g.orientation_) orientation_ = g.orientation_;
        if(g.velocity_) velocity_ = g.velocity_;
        if(g.rotation_) rotation_ = g.rotation_;
    }
};
