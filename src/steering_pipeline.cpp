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

    bool goal::can_merge_goals(const goal& g) const {
        auto p = position_ && g.position_;
        auto o = orientation_ && g.orientation_;
        auto v = velocity_ && g.velocity_;
        auto r = rotation_ && g.rotation_;
        return !(p || o || v || r);
    }

    real path_with_goal::get_max_priority() {
        return (character_->get_position() - goal_.position_).magnitude();
    }

    steering_pipe::steering_pipe() : fallback_(nullptr), constraint_steps_(100), path_(nullptr) {}

    steering_pipe::set_actuator(std::shared_ptr<actuator> a) {
        actuator_ = a;
        path = nullptr;
    }

    steering_pipe::get_steering(std::shared_ptr<steering> steer) {
        goal g;

        // from an empty goal, accumulate a goal from the targeters
        for(auto & t : targeters_) {
            goal targeter_result = t->get_goal();
            if(g.can_merge_goals(targeter_result)) {
                g.update_goal(targeter_result);
            }
        }

        // break down the goal into smaller components
        for(auto & d : decomposers_) {
            g = d->decompose_goal(g);
        }

        // create an empty path
        if(!path_) path_ = actuator_->create_path_object();

        real shortest_violation, current_violation, max_violation;
        std::shared_ptr<constraint> violating_constraint;
        for(unsigned i = 0; i < constraint_steps_, ++i) {
            // find the path to this goal
            actuator_->get_path(path_, g);
            max_violation = shortest_violation = path_->get_max_priority();
            // find a constraint that is violated first.
            for(auto & c : constraints_) {
                // clear the flags that indicate the constraints used.
                if(i == 0) c->suggestion_used_ = false;
                // check to see if this constraint is violated earlier than any other
                current_violation = c->will_violate(path_, shortest_violation);
                if(current_violation > 0 && current_violation < shortest_violation) {
                    shortest_violation = current_violation;
                    violating_constraint = c;
                }
            }
            
            // check if we found a violation.
            if(shortest_violation < max_violation) {
                // update the goal and check constraints again.
                g = violating_constraint->suggest(path_);
                violating_constraint->suggestion_used_ = true;
            } else {
                actuator_->get_steering(steer, path_);
                return;
            }
        }
        // We have run out of constraint iterations, use fallback.
        if(fallback_) fallback_->get_steering(steer);
    }
};
