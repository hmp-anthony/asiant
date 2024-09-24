#ifndef SEEK_HPP
#define SEEK_HPP

#include <asiant/kinematic.hpp>
#include <asiant/steering.hpp>

namespace asiant {
    class seek {
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
        
        kinematic& get_character() {
            return character;
        }

        kinematic& get_target() {
            return target;
        }
        
        virtual void update() = 0;
    protected:
        kinematic character;
        kinematic target;
        real max_speed;
    };
    
    class seek_with_max_speed : public seek {
    public:
        void update() {
            auto velocity = target.get_position() - character.get_position();
            velocity.normalize();
            velocity *= max_speed;
            character.set_velocity(velocity);
            character.update_to_face_velocity();
        }
    };

    class seek_with_satisfaction_radius : public seek {
    public:
        void set_radius(real r) {
            radius = r; 
        }
        void set_time_to_target(real t) {
            time_to_target = t;
        }

        void update() {
            auto velocity = target.get_position() - character.get_position();
            if(velocity.magnitude() < radius) {
                character.update_to_face_velocity(velocity); 
                velocity = vector(0,0,0);
                character.set_velocity(velocity);
                return;
            }
            velocity *= (1.0/time_to_target);
            if(velocity.magnitude() > max_speed) {
                velocity.normalize();
                velocity *= max_speed;
            }
            character.set_velocity(velocity);
            character.update_to_face_velocity(); 
        }
    private:
        real radius;
        real time_to_target;
    };

    class seek_with_max_acceleration : public seek {
    public:
        void update() {
        }
    };
};

#endif
