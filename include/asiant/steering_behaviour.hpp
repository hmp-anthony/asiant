#ifndef STEERING_BEHAVIOUR_HPP
#define STEERING_BEHAVIOUR_HPP

#include <asiant/precision.hpp>
#include <asiant/vector.hpp>
#include <asiant/kinematic.hpp>
#include <asiant/path.hpp>
#include <asiant/primitives.hpp>

#include <memory>
#include <vector>

namespace asiant {

    class steering_behaviour {
    public:
        std::shared_ptr<kinematic> get_character();
        void set_character(std::shared_ptr<kinematic> c);
        virtual void get_steering(std::shared_ptr<steering> steer) = 0;
    protected:
        std::shared_ptr<kinematic> character_;
    };

    class seek : public steering_behaviour {
    public:
        std::shared_ptr<vector> get_target();
        void set_target(std::shared_ptr<vector> v);
        void set_max_acceleration(real a);
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        std::shared_ptr<vector> target_;
        real max_acceleration_;
    };

    class flee : public seek {
    public:
        virtual void get_steering(std::shared_ptr<steering> steer);
    };
    
    class wander : public seek {
    public:
        void set_max_rotation(real r);
        void set_max_speed(real s);
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        real max_rotation_;
        real max_speed_;
    };

    class seek_with_velocity_radius : public seek {
    public:
        void set_target_radius(real r);
        void set_velocity_radius(real r);
        void set_time_to_target(real t);
        void set_max_speed(real s);
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        real target_radius_;
        real velocity_radius_;
        real time_to_target_;
        real max_speed_;
    };

    class flee_with_velocity_radius : public seek_with_velocity_radius {
    public:
        virtual void get_steering(std::shared_ptr<steering> steer);
    };

    class avoid_sphere : public seek {
    public:
        void set_sphere(std::shared_ptr<sphere> s);
        void set_avoid_margin(real m);
        void set_max_look_ahead(real m);
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        std::shared_ptr<sphere> sphere_;
        real avoid_margin_;
        real max_look_ahead_;
    }

    class follow_path_seek : public seek_with_velocity_radius {
    public:
        follow_path_seek();
        void set_path_constrained_entity(std::shared_ptr<path_constrained_entity> p);
        std::shared_ptr<path_constrained_entity> get_path_constrained_entity();
        virtual void get_steering(std::shared_ptr<steering> steer);
    protected:
        std::shared_ptr<path_constrained_entity> path_constrained_entity_;
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

    class priority_steering : public steering_behaviour {
    public:
        void set_epsilon(real e);
        void add_behaviour(std::shared_ptr<steering_behaviour> s);
        void get_steering(std::shared_ptr<steering> steer);
    protected:
        std::vector<std::shared_ptr<steering_behaviour>> behaviours_;
        std::shared_ptr<steering_behaviour> last_used_;
        real epsilon_;
    };
};

#endif
