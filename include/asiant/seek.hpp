#ifndef SEEK_HPP
#define SEEK_HPP

#include <asiant/kinematic.hpp>
#include <asiant/steering.hpp>

#include<iostream>

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
        void set_max_acceleration(real a) {
            max_acceleration = a;
        }

        void update() {
            auto velocity = target.get_position() - character.get_position();
            velocity.normalize();
            velocity *= max_acceleration;
            character.set_velocity(velocity);
        }
    private:
        real max_acceleration;
    };

    class seek_with_velocity_radius : public seek {
    public:
        void set_target_radius(real r) {
            target_radius = r;
        }
        void set_slow_radius(real r) {
            slow_radius = r;
        }
        void set_time_to_target(real r) {
            time_to_target = r;
        }
        void set_max_acceleration(real a) {
            max_acceleration = a;
        }

        void update() {
            auto direction = target.get_position() - character.get_position();
            auto distance = direction.magnitude();
            if(distance < target_radius) {
                // we need to stop moving
                character.update_to_face_velocity(direction); 
                direction = vector(0,0,0);
                character.set_velocity(direction);
                return;
            }
            real target_speed;
            if(distance > slow_radius) {
                target_speed = max_speed;
            } else {
                // we are in the slow zone.
                target_speed = max_speed * (distance / slow_radius);
            }

            auto target_velocity = direction;
            target_velocity.normalize();
            target_velocity *= target_speed;
            
            target_velocity *= (1.0 / time_to_target);
            
            if(target_velocity.magnitude() > max_acceleration) {
                target_velocity.normalize();
                target_velocity *= max_acceleration;
            }
           
            character.set_velocity(target_velocity);
            character.update_to_face_velocity(direction); 
        }
    private:
        real target_radius;
        real slow_radius; // this must be larger than target_radius
        real time_to_target;
        real max_acceleration;
    };

    class pursue : public seek_with_max_speed {
    public:
        void set_max_prediction(real p) {
            max_prediction = p;
        }
        kinematic& get_explicit_target() {
            return explicit_target;
        }
        void update() {
            auto direction = target.get_position() - character.get_position();
            auto distance = direction.magnitude();
            auto speed = character.get_velocity().magnitude();

            real prediction;
            if(speed < (distance / max_prediction)) {
                prediction = max_prediction;
            } else {
                prediction = distance / speed;
            }
             
            explicit_target = target;

            auto delta_position = target.get_velocity() * prediction;
            auto position = target.get_position();
            auto new_position = position + delta_position;
            target.set_position(new_position);
            seek_with_max_speed::update();    
        }
    private:
        kinematic explicit_target;
        real max_prediction;
    };

    class follow_path : public seek_with_velocity_radius {
    public:
        void set_path(path& pth) {
            p = pth;
        }
        void set_path_offset(real o) {
            path_offset = o;
        }
        void update() {
           auto current_param = p.get_parameter(character.get_position());
           auto target_parameter = current_param + path_offset;
           auto position = p.get_position(target_parameter);
           target.set_position(position);
           seek_with_velocity_radius::update();
       }
    private:
        path p;
        real path_offset;
    };
};

#endif
