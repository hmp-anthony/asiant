#include <asiant/steering_behaviour.hpp>

namespace asiant {
    void seek::get_steering(std::shared_ptr<steering> steer) {
        auto linear = *target - character->get_position();
        steer->set_linear(linear);
    }

    void flee::get_steering(std::shared_ptr<steering> steer) {
        auto linear = character->get_position() - *target;
        steer->set_linear(linear);
    }

    void blended_steering::get_steering(std::shared_ptr<steering> steer) {
        steer->clear();
        real total_weight = 0;
        auto temp = std::make_shared<steering>();

        vector linear_output = vector(0, 0 ,0);
        auto angular_output = 0;
        for(auto& baw : behaviours) {
            // set character
            baw->behaviour->set_character(character);
            // obtain steering
            baw->behaviour->get_steering(temp);
            linear_output += temp->get_linear() * baw->weight;
            angular_output += temp->get_angular() * baw->weight;
            total_weight += baw->weight;
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
