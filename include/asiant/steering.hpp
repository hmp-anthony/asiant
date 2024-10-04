#ifndef STEERING_HPP
#define STEERING_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>
#include <asiant/kinematic.hpp>

#include <memory>

namespace asiant {
    class steering {
    public:
        steering() {
            linear = vector(0,0,0);
            angular = 0;
        }
        vector get_linear() const {
            return linear;
        }
        real get_angular() const {
            return angular;
        }
        void set_linear(const vector& v) {
            linear = v;
        }
        void set_angular(const real r) {
            angular = r;
        }

        real square_magnitude() {
            return linear.square_magnitude() + angular * angular;
        }

        void clear() {
            linear = vector(0,0,0);
            angular = 0;
        }
    private:
        vector linear;
        real   angular;
    };
};

#endif
