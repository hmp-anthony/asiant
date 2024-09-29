#include <asiant/asiant.hpp>

namespace asiant {
    std::shared_ptr<steering> seek::get_steering() {
        auto output = std::make_shared<steering>();
        auto linear = *target - character->get_position();
        output->set_linear(linear);
        return output;
    }

    std::shared_ptr<steering> flee::get_steering() {
        auto output = std::make_shared<steering>();
        auto linear = character->get_position() - *target;
        output->set_linear(linear);
        return output;
    }

    std::shared_ptr<steering> blended_steering::get_steering() {
        auto output = std::make_shared<steering>();

        real total_weight = 0;
        auto temp = std::make_shared<steering>();

        vector linear_output = 0;
        auto angular_output = 0;
        for(auto& baw : behaviours) {
            // set character
            baw.behaviour->set_character(character);
            // obtain steering
            temp = baw.behaviour->get_steering();

            linear_output += temp->get_linear() * baw.weight;
            angular_output += temp->get_angular() * baw.weight;
            total_weight += baw.weight;
        }
        if(total_weight > 0.0) {
            total_weight = (real)1.0/total_weight;
            linear_output *= total_weight;
            angular_output *= total_weight;
        }
        output->set_linear(linear_output);
        output->set_angular(angular_output);
        return output;
    }
}
