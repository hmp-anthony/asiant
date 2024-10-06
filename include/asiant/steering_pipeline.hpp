#ifndef STEERING_PIPELINE_HPP
#define STEERING_PIPELINE_HPP

#include <asiant/path.hpp>
#include <asiant/steering_behaviour.hpp>

#include <optional>

namespace asiant {
    struct goal {
        std::optional<vector> position_;
        std::optional<real> orientation_;
        std::optional<vector> velocity_;
        std::optional<real> rotation_;

        // creates a new goal with no channels.
        goal();
        // clears all channels from the goal.
        void clear();
        // updates the goal by adding additional channels from the given goal.
        void update_goal();
        // checks if the given goal can be merged. basically: if *this and `goal` have the
        // same channel set, we can not merge.
        bool can_merge_goals(const goal& goal) const;
    };

    struct path_with_goal {
        path_character path_character_;
        goal goal_;
        real get_max_priority();
    };

    class steering_pipe;

    class steering_pipe_component {
    protected:
        friend class steering_pipe;
        std::shared_ptr<steering_pipe> steering_pipe_;
    };

    class targeter : public steering_pipe_component {
    public:
        virtual goal get_goal() = 0;
    };

    class decomposer : public steering_pipe_component {
    public:
        virtual goal decompose_goal() = 0;
    };

    class constraint : public steering_pipe_component {
    public:
        bool suggestion_used;
        virtual real will_violate(const std::shared_ptr<path_with_goal> pwg, real max_priority) = 0;
        virtual goal suggest(const std::shared_ptr<path_with_goal> pwg) = 0;
    };

    class actuator : public steering_pipe_component {
    public:
        virtual std::shared_ptr<path_with_goal> create_path_object() = 0;
        virtual void get_path(std::shared_ptr<path_with_goal> p, const goal& goal) = 0;
        virtual void get_steering(std::shared_ptr<steering> steer,
                                  const shared_ptr<path_with_goal> p) = 0;
    };
    
    class steering_pipe : public steering_behaviour {
    public:
        steering_pipe();
        void set_actuator(std::shared_ptr<actuator> a);
        void get_steering(std::shared_ptr<steering> steer);
        void register_components();

        std::vector<std::shared_ptr<targeter>> targeters_;
        std::vector<std::shared_ptr<decomposer>> decomposers_;
        std::vector<std::shared_ptr<constraint>> constraints_;
        
        unsigned constraint_steps_;

        std::shared_ptr<steering_behaviour> fallback_;
        std::shared_ptr<path_with_goal> path_;

    private:
        std::shared_ptr<actuator> actuator_;
    };

    class fixed_goal_targeter : public targeter {
    public:
        goal goal_;
        virtual goal get_goal();
    };
    
    class avoid_spheres_constraint : public constraint {
    public:
        std::vector<std::shared_ptr<sphere>> spheres_;
        real avoid_margin_;
        virtual real will_violate(const std::shared_ptr<path_with_goal> pwg, real max_priority);
        virtual goal suggest(const std::shared_ptr<path_with_goal> pwg);
    private:
        goal suggestion_;
        real will_violate(const std::shared_ptr<path_with_goal> p, 
                          real max_priority,
                          sphere& obstacle);
    };

    class basic_actuator : actuator {
    public:
        real max_acceleration_;
        virtual std::shared_ptr<path_with_goal> create_path_object();
		virtual void get_path(std::shared_ptr<path_with_goal>  path, const goal& goal);
		virtual void get_steering(std::shared_ptr<steering> output, 
                                  const std::shared_ptr<path_with_goal> path);
    private:
        seek seek_;
    };
};

#endif
