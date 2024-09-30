#include <asiant/steering_behaviour.hpp>

namespace asiant {
    void seek::get_steering(std::shared_ptr<steering> steer) {
        auto linear = *target_ - character_->get_position();
        steer->set_linear(linear);
    }

    void flee::get_steering(std::shared_ptr<steering> steer) {
        auto linear = character_->get_position() - *target_;
        steer->set_linear(linear);
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
