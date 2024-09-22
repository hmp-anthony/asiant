#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include <asiant/vector.hpp>
#include <asiant/steering.hpp>

namespace asiant {
    class kinematic {
    public:
        kinematic() {}
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
    
        void update(steering s, real time) {
            position += velocity * time;
            orientation += rotation * time;
            velocity += s.get_linear() * time;
            rotation += s.get_angular() * time;
        }
    private:
        vector position;
        real   orientation;
        vector velocity;
        real   rotation;
    };
};

#endif
