#ifndef WANDER_HPP
#define WANDER_HPP

#include <asiant/kinematic.hpp>
#include <asiant/steering.hpp>
#include <asiant/precision.hpp>

#include <random>

namespace asiant {
    class wander {
    public:
        void set_max_speed(real max) {
            max_speed = max;
        }
        
        void set_max_rotation(real r) {
            max_rotation = r;
        }

        void set_character(kinematic& k) {
            character = k;
        }
        kinematic& get_character() {
            return character;
        }

        void update() { 
            auto o = character.get_orientation();
            auto v = vector(-1 * max_speed * real_sin(o), max_speed * real_cos(o), 0);
            character.set_velocity(v);
            auto r = random_real() * max_rotation;
            character.set_rotation(r);
        }
    private:
        real random_real() {
            // return a random number between -1 and 1.
            std::random_device seeder;
            std::mt19937 engine(seeder());
            std::uniform_real_distribution<real> dist(-1, 1);
            return dist(engine);
        }

        kinematic character;
        real max_speed;
        real max_rotation;
    };
    
};

#endif
