#ifndef STEERING_HPP
#define STEERING_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>

namespace asiant {
    class steering {
    public:
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

    private:
        vector linear;
        real   angular;
    };
};

#endif
