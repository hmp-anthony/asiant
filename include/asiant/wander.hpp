#ifndef SEEK_HPP
#define SEEK_HPP

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
        
        void set_target(kinematic& k) {
            target = k;
        }
        
        kinematic& get_character() {
            return character;
        }

        kinematic& get_target() {
            return target;
        }

        void update() { 
            auto o = character.get_orientation();
            auto v = vector(max_speed * real_cos(o), max_speed * real_sin(o), 0);
            character.set_velocity(v);
            r = random_real() * max_rotation;
            character.set_rotation(r);
        }
    private:
        real random_real() {
            // return a random number between -1 and 1.
        }

        kinematic character;
        real max_speed;
        real max_rotation;
    };
    
};

#endif
