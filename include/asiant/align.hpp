#ifndef ALIGN_HPP
#define ALIGN_HPP

#include <asiant/kinematic.hpp>
#include <asiant/utils.hpp>

namespace asiant {
    class align {
    public:
        void set_character(kinematic& c) {
            character = c;
        }
        void set_target(kinematic& t) {
            target = t;
        }
        void set_max_angular_acceleration(real a) {
            max_angular_acceleration = a;
        }
        void set_max_rotation(real r) {
            max_rotation = r;
        }
        void set_target_radius(real r) {
            target_radius = r;
        }
        void set_slow_radius(real r) {
            slow_radius = r;
        }
        void set_time_to_target(real t) {
            time_to_target = t;
        }
        kinematic& get_character() {
            return character;
        }
        kinematic& get_target() {
            return target;
        }
        
        void update() {
            rotation = target.get_orientation() - character.get_orientation();
            rotation = utils::map_to_range_pi_minus_pi(rotation);
        }
    private:
        kinematic character;
        kinematic target;
        real max_angular_acceleration;
        real max_rotation;
        real target_radius;
        real slow_radius;
        real time_to_target;
    };
};

#endif
