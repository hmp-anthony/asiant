#ifndef STEERING_BEHAVIOUR_HPP
#define STEERING_BEHAVIOUR_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>
#include <asiant/kinematic.hpp>

#include <memory>
#include <vector>

namespace asiant {

    class steering_behaviour {
    public:
        std::shared_ptr<kinematic> get_character() {
            return character;
        }
        void set_character(std::shared_ptr<kinematic> c) {
            character = c;
        }
        virtual std::shared_ptr<steering> get_steering() = 0;
    protected:
        std::shared_ptr<kinematic> character;
    };

    class seek : public steering_behaviour {
    public:
        std::shared_ptr<const vector> get_target() {
            return target;
        }
        void set_target(std::shared_ptr<vector> v) {
            target = v;
        }
        void set_max_acceleration(real a) {
            max_acceleration = a;
        }
        real max_acceleration;
        virtual std::shared_ptr<steering> get_steering();
    protected:
        std::shared_ptr<const vector> target;
    };

    class flee : public seek {
    public:
        virtual std::shared_ptr<steering> get_steering();
    };

    class blended_steering : public steering_behaviour {
    public:
        struct behaviour_and_weight {
            std::shared_ptr<steering_behaviour> behaviour;
            real weight;
        };

        std::vector<std::shared_ptr<behaviour_and_weight>> behaviours;
        virtual std::shared_ptr<steering> get_steering();
    };
};

#endif
