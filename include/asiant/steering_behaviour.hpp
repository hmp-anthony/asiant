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
            return character_;
        }
        void set_character(std::shared_ptr<kinematic> c) {
            character_ = c;
        }
        virtual void get_steering(std::shared_ptr<steering> steer) = 0;
    protected:
        std::shared_ptr<kinematic> character_;
    };

    class seek : public steering_behaviour {
    public:
        std::shared_ptr<const vector> get_target() {
            return target_;
        }
        void set_target(std::shared_ptr<vector> v) {
            target_ = v;
        }
        void set_max_acceleration(real a) {
            max_acceleration_ = a;
        }
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        std::shared_ptr<const vector> target_;
        real max_acceleration_;
    };

    class flee : public seek {
    public:
        virtual void get_steering(std::shared_ptr<steering> steer);
    };

    class blended_steering : public steering_behaviour {
    public:
        struct behaviour_and_weight {
            std::shared_ptr<steering_behaviour> behaviour_;
            real weight_;
        };

        std::vector<std::shared_ptr<behaviour_and_weight>> behaviours_;
        virtual void get_steering(std::shared_ptr<steering> s);
    };
};

#endif
