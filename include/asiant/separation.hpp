#ifndef SEPARATION_HPP
#define SEPARATION_HPP

#include <asiant/kinematic.hpp>

#include <vector>

namespace asiant {
    class separation {
    public:
        void set_character(kinematic& c) {
            character = c;
        }
        void set_targets(std::vector<kinematic>& ts) {
            targets = ts;
        }
        void set_threshold(real t) {
            threshold = t;
        }
        void set_decay_coefficient(real dc) {
            decay_coefficient = dc;
        }
        void set_max_acceleration(real a) {
            max_acceleration = a;
        }
        kinematic& get_character() {
            return character;
        }
        std::vector<kinematic>& get_targets() {
            return targets;
        }
        void update() {
            auto resulting_velocity = vector(0.0, 0.0, 0.0);
            for(auto& target : targets) {
                auto direction = character.get_position() - target.get_position();
                auto distance = direction.magnitude();
                if(distance < threshold) {
                    auto c = decay_coefficient / (distance * distance);
                    auto strength = std::min(c, max_acceleration);
                    direction.normalize();
                    resulting_velocity += direction * strength;
                }
            }
            auto final_velocity  = character.get_velocity() + resulting_velocity;
            character.set_velocity(final_velocity);
            character.update_to_face_velocity();
        }
    private:
        kinematic character;
        std::vector<kinematic> targets;
        real threshold;
        real decay_coefficient;
        real max_acceleration;
    };
};

#endif 
