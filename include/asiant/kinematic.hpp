#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include <asiant/vector.hpp>
#include <asiant/steering.hpp>

namespace asiant {
    class kinematic {
    public:
        kinematic() {
            orientation = 0;
            rotation = 0;
        }
        kinematic(vector pos) {
            position = pos;
            orientation = 0;
            velocity = vector(0.0, 0.0, 0.0);
            rotation = 0;
        }

        vector get_position() {
            return position;
        }

        real get_orientation() {
            return orientation;
        }

        void set_position(vector& v) {
            position = v;
        }

        void set_velocity(vector& v) {
            velocity = v;
        }

        void set_orientation(real o) {
            orientation = o;
        }

        void set_rotation(real r) {
            rotation = r;
        }

        void update(real time) {
            position += velocity * time;
            orientation += rotation * time;
        }

        void update(steering s, real time) {
            position += velocity * time;
            orientation += rotation * time;
            velocity += s.get_linear() * time;
            rotation += s.get_angular() * time;
        }

        void update(steering s, real max_speed, real time) {
            position += velocity * time;
            orientation += rotation * time;
            velocity += s.get_linear() * time;
            rotation += s.get_angular() * time;

            if(velocity.magnitude() > max_speed) {
                velocity.normalize();
                velocity *= max_speed;
            }
        }

        void update_to_face_velocity() {
            orientation = real_atan(-1 * velocity[0], velocity[1]);
        }

        void update_to_face_velocity(vector vel) {
            orientation = real_atan(-1 * vel[0], vel[1]);
        }

        void update_to_face_velocity(steering s, real time) {
            position += velocity * time;
            velocity += s.get_linear() * time;
            auto velocity_direction = real_atan(-1 * velocity[0], velocity[1]);
            orientation = velocity_direction;
        }

        void smooth_update_to_face_velocity(steering s, real time, int smoothness) {
            position += velocity * time;
            velocity += s.get_linear() * time;
            auto velocity_direction = real_atan(-1 * velocity[0], velocity[1]);
            orientation = orientation + (velocity_direction - orientation) / (float)smoothness;
        }

        
    private:
        vector position;
        real   orientation;
        vector velocity;
        real   rotation;
    };
};

#endif
