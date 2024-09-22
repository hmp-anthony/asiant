#ifndef KINEMATIC_HPP
#define KINEMATIC_HPP

#include<asiant/vector.hpp>

namespace asiant {
    class kinematic {
    public:
        kinematic(vector position,
                  real   orientation,
                  vector velocity
                  real   rotation);
    private:
        vector position;
        real   orientation;
        vector velocity;
        real   rotation;
    };
};

#endif
