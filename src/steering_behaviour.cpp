#include <asiant/steering_behaviour.hpp>
#include <asiant/utils.hpp>

namespace asiant {
    std::shared_ptr<kinematic> steering_behaviour::get_character() {
        return character_;
    }

    void steering_behaviour::set_character(std::shared_ptr<kinematic> c) {
        character_ = c;
    }
    
    std::shared_ptr<vector> seek::get_target() {
        return target_;
    }

    void seek::set_target(std::shared_ptr<vector> v) {
        target_ = v;
    }

    void seek::set_max_acceleration(real a) {
        max_acceleration_ = a;
    }

    void seek::get_steering(std::shared_ptr<steering> steer) {
        auto linear = *target_ - character_->get_position();
        if(linear.square_magnitude() > 0) {
            linear.normalize();
            linear *= max_acceleration_;
        }
        steer->set_linear(linear);
    }

    void flee::get_steering(std::shared_ptr<steering> steer) {
        auto linear = character_->get_position() - *target_;
        if(linear.square_magnitude() > 0) {
            linear.normalize();
            linear *= max_acceleration_;
        }
        steer->set_linear(linear);
    }

    void wander::set_max_rotation(asiant::real r) {
        max_rotation_ = r;
    }

    void wander::set_max_speed(asiant::real s) {
        max_speed_ = s;
    }

    void wander::get_steering(std::shared_ptr<steering> steer) {
        if(!target_) {
            target_ = std::make_shared<vector>();
        }
        //get a random angle between -pi and pi;
        auto o = random_binomial(1) * max_rotation_;
        //get a random number to move the target;
        auto r = random_real(1) * max_speed_;
        auto delta = vector(r * real_sin(o), r * real_cos(o), 0);
        *target_ = character_->get_position() + delta;
        seek::get_steering(steer);
    }

    void seek_with_velocity_radius::set_target_radius(real r) {
        target_radius_ = r;
    }

    void seek_with_velocity_radius::set_velocity_radius(real r) {
        velocity_radius_ = r;
    }

    void seek_with_velocity_radius::set_time_to_target(real t) {
        time_to_target_ = t;
    }

    void seek_with_velocity_radius::set_max_speed(real s) {
        max_speed_ = s;
    }

    void seek_with_velocity_radius::get_steering(std::shared_ptr<steering> steer) {
        auto direction = *target_ - character_->get_position();
        auto distance = direction.magnitude();
        // if we are inside the target radius then we do nothing.
        if(distance < target_radius_) {
            steer->clear();
            return;
        }

        real target_speed = 0;
        if(distance > velocity_radius_) {
            // if we are outside the velocity radius then we approach at full speed.
            target_speed = max_speed_;
        } else {
            // we want to factor to linearly increase inside the velocity/target annulus. 
            auto factor = (distance - target_radius_) / (velocity_radius_ - target_radius_);
            target_speed = max_speed_ * factor;
        }

        auto target_velocity = direction;
        target_velocity.normalize();
        target_velocity *= target_speed;
       
        target_velocity *= (1.0 / time_to_target_);
        
        if(target_velocity.magnitude() > max_acceleration_) {
            target_velocity.normalize();
            target_velocity *= max_acceleration_;
        }

        steer->set_linear(target_velocity);
    }

    void flee_with_velocity_radius::get_steering(std::shared_ptr<steering> steer) {
        auto direction = character_->get_position() - *target_;
        auto distance = direction.magnitude();
        // if we are outside the larger of the radi - do nothing.
        if(distance > velocity_radius_) {
            steer->clear();
            return;
        }

        real target_speed = 0;
        if(distance < target_radius_) {
            // if we are in the target_radius we are close (very, relatively speaking).
            // We need to get out as quick as we can.
            target_speed = max_speed_;
        } else {
            // we want to factor to linearly decrease inside the velocity/target annulus. 
            auto factor = (velocity_radius_ - distance) / (velocity_radius_ - target_radius_);
            target_speed = max_speed_ * factor;
        }

        auto target_velocity = direction;
        target_velocity.normalize();
        target_velocity *= target_speed;
       
        target_velocity *= (1.0 / time_to_target_);
        
        if(target_velocity.magnitude() > max_acceleration_) {
            target_velocity.normalize();
            target_velocity *= max_acceleration_;
        }

        steer->set_linear(target_velocity);
    }

    follow_path_seek::follow_path_seek() {
        target_ = std::make_shared<vector>();
    }

    void follow_path_seek::set_path_constrained_entity(std::shared_ptr<path_constrained_entity> p) {
        path_constrained_entity_ = p;
    }

    std::shared_ptr<path_constrained_entity> follow_path_seek::get_path_constrained_entity() {
        return path_constrained_entity_;
    }
        
    void follow_path_seek::get_steering(std::shared_ptr<steering> steer) {
        auto position = path_constrained_entity_->get_position();
        *target_ = position;
        seek_with_velocity_radius::get_steering(steer);
    }

    void blended_steering::get_steering(std::shared_ptr<steering> steer) {
        steer->clear();
        real total_weight = 0;
        auto temp = std::make_shared<steering>();

        vector linear_output = vector(0, 0 ,0);
        auto angular_output = 0;
        for(auto& baw : behaviours_) {
            // set character
            baw->behaviour_->set_character(character_);
            // obtain steering
            baw->behaviour_->get_steering(temp);
            linear_output += temp->get_linear() * baw->weight_;
            angular_output += temp->get_angular() * baw->weight_;
            total_weight += baw->weight_;
        }
        if(total_weight > 0.0) {
            total_weight = (real)1.0/total_weight;
            linear_output *= total_weight;
            angular_output *= total_weight;
        }
        steer->set_linear(linear_output);
        steer->set_angular(angular_output);
    }
};
