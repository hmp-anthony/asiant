#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include <asiant/vector.hpp>
#include <asiant/steering.hpp>

#include <memory>

namespace asiant {
    class kinematic {
    public:
        kinematic() {
            position = std::make_shared<vector>();
            velocity = std::make_shared<vector>();
            orientation = 0;
            rotation = 0;
        }

        kinematic(vector p) {
            position = std::make_shared<vector>();
            *position = p;
            orientation = 0;
            velocity = std::make_shared<vector>();
            rotation = 0;
        }

        vector get_position() {
            return *position;
        }

        std::shared_ptr<vector> get_position_pointer() {
            return position;
        }

        real get_orientation() {
            return orientation;
        }
        
        vector get_orientation_as_vector() const {
            return vector(-1.0 * real_sin(orientation), real_cos(orientation), 0.0);
        }

        vector get_velocity() {
            return *velocity;
        }

        void set_position(vector& v) {
            *position = v;
        }
        
        void set_position(vector&& v) {
            (*position)[0] = v[0];
            (*position)[1] = v[1];
            (*position)[2] = v[2];
        }

        void set_velocity(vector& v) {
            *velocity = v;
        }  

        void set_velocity(vector&& v) {
            (*velocity)[0] = v[0];
            (*velocity)[1] = v[1];
            (*velocity)[2] = v[2];
        }
       
        void set_orientation(real o) {
            orientation = o;
        }

        void set_rotation(real r) {
            rotation = r;
        }

        void update(real time) {
            *position += *velocity * time;
            orientation += rotation * time;
        }

        void update(real time, real max_speed) {
            *position += *velocity * time;
            orientation += rotation * time;
            if((*velocity).square_magnitude() > max_speed * max_speed) {
                (*velocity).normalize();
                (*velocity) *= max_speed;
            }
        }

        void update(std::shared_ptr<steering> s, real time) {
            *position += *velocity * time;
            orientation += rotation * time;
            *velocity += s->get_linear() * time;
            rotation += s->get_angular() * time;
        }

        void update(std::shared_ptr<steering> s, real max_speed, real time) {
            *position += *velocity * time;
            orientation += rotation * time;
            *velocity += s->get_linear() * time;
            rotation += s->get_angular() * time;

            if((*velocity).magnitude() > max_speed) {
                (*velocity).normalize();
                (*velocity) *= max_speed;
            }
        }

        void update_to_face_velocity() {
            orientation = real_atan(-1 * (*velocity)[0], (*velocity)[1]);
        }

        void update_to_face_velocity(vector vel) {
            orientation = real_atan(-1 * vel[0], vel[1]);
        }

        void update_to_face_velocity(steering& s, real time) {
            *position += *velocity * time;
            *velocity += s.get_linear() * time;
            auto velocity_direction = real_atan(-1 * (*velocity)[0], (*velocity)[1]);
            orientation = velocity_direction;
        }

        vector get_orientation_as_vector() {
            return vector(std::cos(orientation), std::sin(orientation), 0);
        }

        void integrate(const steering& s, real drag, real duration) {
            (*position)[0] += (*velocity)[0] * duration;
            (*position)[1] += (*velocity)[1] * duration;
            (*position)[2] += (*velocity)[2] * duration;
            orientation += rotation*duration;

            drag = std::pow(drag, duration);
            *velocity *= drag;
            rotation *= drag * drag;

            auto linear = s.get_linear();
            (*velocity)[0] += linear[0] * duration;
            (*velocity)[1] += linear[1] * duration;
            (*velocity)[2] += linear[2] * duration;
            rotation += s.get_angular() * duration;

        }

        void trim_max_speed(real max_speed) {
            if ((*velocity).magnitude() > max_speed) {
                (*velocity).normalize();
                *velocity *= max_speed;
            }
        }
    private:
        std::shared_ptr<vector> position;
        real                    orientation;
        std::shared_ptr<vector> velocity;
        real                    rotation;
    };
};

#endif
