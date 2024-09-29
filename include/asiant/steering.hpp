#ifndef STEERING_HPP
#define STEERING_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>

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
    private:
        vector linear;
        real   angular;
    };

    class steering_behaviour {
    public:
        std::shared_ptr<kinematic> get_character() {
            return character;
        }
        virtual std::shared_ptr<steering> get_steering() = 0;
    protected:
        std::shared_ptr<kinematic> character;
    };

    class seek : public steering_behaviour {
    public:
        std::shared_ptr<vector> get_target() {
            return target;
        }
        virtual std::shared_ptr<steering> get_steering();
    protected:
        std::shared_ptr<const vector> target;
    }

    class flee : public seek {
    public:
        virtual std::shared_ptr<steering> get_steering();
    }
};

#endif
