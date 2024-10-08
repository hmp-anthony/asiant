#include <asiant/steering_pipeline.hpp>

#include <limits>

namespace asiant {

    goal::goal() {
        clear();
    }
    
    void goal::clear() {
        position_.reset();
        orientation_.reset();
        velocity_.reset();
        rotation_.reset();
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
        return (character_->get_position() - goal_.position_.value()).magnitude();
    }

    steering_pipe::steering_pipe() : fallback_(nullptr), constraint_steps_(100), path_(nullptr) {}

    void steering_pipe::set_actuator(std::shared_ptr<actuator> a) {
        actuator_ = a;
        path_ = nullptr;
    }

    void steering_pipe::get_steering(std::shared_ptr<steering> steer) {
        goal g;

        // from an empty goal, accumulate a goal from the targeters.
        // we obtain the top level goal.
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
        for(unsigned i = 0; i < constraint_steps_; ++i) {
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

    void steering_pipe::register_components() {
        for(auto & t : targeters_) {
            t->steering_pipe_ = shared_from_this();
        }
        for(auto & d : decomposers_) {
            d->steering_pipe_ = shared_from_this();
        }
        for(auto & c : constraints_) {
            c->steering_pipe_ = shared_from_this();
        }
        actuator_->steering_pipe_ = shared_from_this();
    }
    
    goal fixed_goal_targeter::get_goal() {
        return goal_;
    }

    real avoid_spheres_constraint::will_violate(const std::shared_ptr<path_with_goal> path,
                                                real max_priority) {
        real priority = std::numeric_limits<real>::max();
        real this_priority;
        for(auto & s : spheres_) {
            this_priority = will_violate(path, priority, *s);
            if(this_priority < priority) priority = this_priority;
        }
        return priority;
    }
    
    real avoid_spheres_constraint::will_violate(const std::shared_ptr<path_with_goal> path,
                                                real max_priority,
                                                sphere& s) {

        // make sure we've got a positional goal
		if (!path->goal_.position_) return std::numeric_limits<real>::max();

		// alias the character object
		const std::shared_ptr<kinematic> character = steering_pipe_->get_character();

		// work out where we're going
		vector direction = path->goal_.position_.value() - character->get_position();

		// make sure we're moving
		if (direction.square_magnitude() <=0 ) return std::numeric_limits<real>::max();
	    // find the distance from the line we're moving along to the sphere.
		vector movement_normal = direction.unit();
	    vector character_to_sphere = s.center_ - character->get_position();
 
		real distance_squared = character_to_sphere * movement_normal;
		distance_squared = character_to_sphere.square_magnitude() - 
				distance_squared*distance_squared;

		// Check for collision
		real radius = s.radius_ + avoid_margin_;
		if (distance_squared > radius*radius) return std::numeric_limits<real>::max();
		// Find how far along our movement vector the closest pass is
		real distance_to_closest = character_to_sphere * movement_normal;
       
		// Make sure this isn't behind us and is closer than our lookahead.
	    if (distance_to_closest < 0 || distance_to_closest > max_priority) {
            return std::numeric_limits<real>::max();
        }
        // Find the closest point
		vector closest_point = 
			character->get_position() + movement_normal*distance_to_closest;

		// Find the point of avoidance
		suggestion_.position_ = 
			s.center_ + (closest_point - s.center_).unit() * (s.radius_ + avoid_margin_);
		return distance_to_closest;
	}

	goal avoid_spheres_constraint::suggest(const std::shared_ptr<path_with_goal> pwg)
	{
		return suggestion_;
	}

    std::shared_ptr<path_with_goal> basic_actuator::create_path_object()
	{
        auto p = std::make_shared<path_with_goal>();
		return p;
	}

	void basic_actuator::get_path(std::shared_ptr<path_with_goal> path, const goal& g)
	{
		path->character_ = steering_pipe_->get_character();
		path->goal_ = g;
	}

	void basic_actuator::get_steering(std::shared_ptr<steering> steer, 
                                     const std::shared_ptr<path_with_goal> path)
	{
		if (path->goal_.position_) 
		{
			seek_.set_character(steering_pipe_->get_character());
            // TODO - REALLY?! think about this.
            auto t = std::make_shared<vector>(path->goal_.position_.value());
			seek_.set_target(t);
			seek_.set_max_acceleration(max_acceleration_);
			seek_.get_steering(steer);
		}
		else
		{
			steer->clear();
		}
	}

};
