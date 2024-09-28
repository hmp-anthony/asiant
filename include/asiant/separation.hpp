#ifndef SEPARATION_HPP
#define SEPARATION_HPP

namespace asiant {
    class separation {
    public:
        void update() {

        }
    private:
        kinematic character;
        std::vector<kinematic> targets;
        real threshold;
        real delay coefficient;
        real max_acceleration;
    };
};

#endif 
