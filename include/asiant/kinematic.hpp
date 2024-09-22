#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include <asiant/vector.hpp>
#include <asiant/steering.hpp>

namespace asiant {
    class kinematic {
    public:
        kinematic(vector pos) {
            position = pos;
            orientation = 0;
            velocity = vector(0.0, 0.0, 0.0);
            rotation = 0;
        }

        void update(steering s, real time) {
            position += velocity * time;
            orientation += rotation * time;
            velocity += steering.linear * time;
            rotation += steering.angular * time;
        }
    private:
        vector position;
        real   orientation;
        vector velocity;
        real   rotation;
    };
};

#endif
