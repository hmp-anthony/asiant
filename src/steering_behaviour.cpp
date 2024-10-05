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

    void avoid_sphere::set_sphere(std::shared_ptr<sphere> s) {
        sphere_ = s;
    }

    void avoid_sphere::set_avoid_margin(real m) {
        avoid_margin_ = m;
    }

    void avoid_sphere::set_max_look_ahead(real m) {
        max_look_ahead_ = m;
    }

    void avoid_sphere::get_steering(std::shared_ptr<steering> steer) {
        if(!target_) {
            target_ = std::make_shared<vector>();
        }

        steer->clear();
        if(character_->get_velocity().square_magnitude() <= 0) {
            return;
        }

        // the code in the original repo is different to what follows and
        // is it mathematically incorrect. Even though it is incorrect, it
        // is more efficient as it calculates square_mag instead of mag. 
        vector movement_normal = character_->get_velocity().unit();
        vector character_to_sphere_center = sphere_->center_ - character_->get_position();

        // this tells us how much the char_to_sphere vector projects onto
        // the movement vector.
        auto projection = movement_normal * character_to_sphere_center;
        auto distance = character_to_sphere_center.magnitude() - projection;

        auto radius = sphere_->radius_ + avoid_margin_;
        if(distance >= radius) {
            // if we are clear of the sphere, do nothing.
            return;
        }
        if(projection < 0 || projection > max_look_ahead_) {
            // if the sphere is behind the direction of motion, do nothing.
            return;
        }

        auto closest_point = character_->get_position() + movement_normal * projection;
        auto delta = (closest_point - sphere_->center_).unit();
        *target_ = sphere_->center_ + delta * (sphere_->radius_ + avoid_margin_); 
        seek::get_steering(steer);
    }

    follow_path_seek::follow_path_seek() {
        target_ = std::make_shared<vector>();
    }

    void follow_path_seek::set_path_character(std::shared_ptr<path_character> p) {
        path_character_ = p;
    }

    std::shared_ptr<path_character> follow_path_seek::get_path_character() {
        return path_character_;
    }
        
    void follow_path_seek::get_steering(std::shared_ptr<steering> steer) {
        auto position = path_character_->get_position();
        *target_ = position;
        seek_with_velocity_radius::get_steering(steer);
    }

    void blended_steering::get_steering(std::shared_ptr<steering> steer) {
        steer->clear();
        real total_weight = 0;

        auto temp = std::make_shared<steering>();
        vector linear_output = steer->get_linear();
        auto angular_output = steer->get_angular();

        for(auto& baw : behaviours_) {
            // set character
            baw->behaviour_->set_character(character_);
            // obtain steering
            baw->behaviour_->get_steering(temp);
            linear_output += temp->get_linear() * baw->weight_;
            angular_output += temp->get_angular() * baw->weight_;
            total_weight += baw->weight_;
        }
        steer->set_linear(linear_output);
        steer->set_angular(angular_output);
    }

    void priority_steering::set_epsilon(real e) {
        epsilon_ = e;
    }

    void priority_steering::add_behaviour(std::shared_ptr<steering_behaviour> b) {
        behaviours_.push_back(b);
    }
    
    std::shared_ptr<steering_behaviour> priority_steering::get_last_used() {
        return last_used_;
    }

    void priority_steering::get_steering(std::shared_ptr<steering> steer) {
        real epsilon_squared = epsilon_ * epsilon_;
        steer->clear();
        for(auto& behaviour : behaviours_) {
            behaviour->set_character(character_);
            behaviour->get_steering(steer);
            if(steer->square_magnitude() > epsilon_squared) {
                last_used_ = behaviour;
                return;
            }
        }
    }
};
