#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include <asiant/kinematic.hpp>
#include <asiant/steering.hpp>

namespace asiant {
    class seek_with_max_speed {
    public:
        void set_max_speed(real max) {
            max_speed = max;
        }

        void set_character(kinematic& k) {
            character = k;
        }
        
        void set_target(kinematic& k) {
            target = k;
        }
        
        kinematic& get_character() const {
            return character;
        }

        kinematic& get_target() const {
            return target;
        }

        void update() {
            auto velocity = target.get_position() - character.get_position();
            velocity.normalize();
            velocity *= max_speed;
            character.set_velocity(velocity);
            character.update_to_face_velocity();
        }
    private:
        kinematic character;
        kinematic target;
        real max_speed;
    };
};

#endif
