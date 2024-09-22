#ifndef STEERING_HPP
#define STEERING_HPP

namespace asiant {
    class steering {
    public:
        vector get_linear() const {
            return linear;
        }
        real get_angular() const {
            return angular;
        }
    private:
        vector linear;
        real   angular;
    };
};

#endif
