#include <asiant/steering_behaviour.hpp>

#include <iostream>

namespace asiant {
    std::shared_ptr<steering> seek::get_steering() {
        std::cout << "in seek" << std::endl;
        auto output = std::make_shared<steering>();
        auto linear = *target - character->get_position();
        output->set_linear(linear);
        return output;
    }

    std::shared_ptr<steering> flee::get_steering() {
        std::cout << "in flee" << std::endl;
        auto output = std::make_shared<steering>();
        auto linear = character->get_position() - *target;
        output->set_linear(linear);
        return output;
    }

    std::shared_ptr<steering> blended_steering::get_steering() {
        std::cout << "in blended_steering" << std::endl;
        auto output = std::make_shared<steering>();

        real total_weight = 0;
        auto temp = std::make_shared<steering>();

        vector linear_output = vector(0, 0 ,0);
        auto angular_output = 0;
        for(auto& baw : behaviours) {
            std::cout << "looping through behavious" << std::endl;
            // set character
            baw->behaviour->set_character(character);
            std::cout << "set_character" << std::endl;
            // obtain steering
            std::cout << baw->behaviour << std::endl;
            temp = baw->behaviour->get_steering();
            std::cout << "got_steering" << std::endl;
            linear_output += temp->get_linear() * baw->weight;
            angular_output += temp->get_angular() * baw->weight;
            total_weight += baw->weight;
        }
        std::cout << "before total_weight" << std::endl;
        if(total_weight > 0.0) {
            total_weight = (real)1.0/total_weight;
            linear_output *= total_weight;
            angular_output *= total_weight;
        }
        output->set_linear(linear_output);
        output->set_angular(angular_output);
        return output;
    }
};
