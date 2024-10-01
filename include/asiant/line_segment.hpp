#ifndef LINE_SEGMENT_HPP
#define LINE_SEGMENT_HPP

#include <asiant/vector.hpp>

namespace asiant {
    class line_segment {
    public:
        line_segment(vector start, vector end) {
            a = start;
            b = end;
        }
        vector& get_start() {
            return a;
        }
        vector& get_end() {
            return b;
        }

        vector operator [](const real t) const {
            return a * (1 - t) + b * t;
        }

        vector at(const real t) const {
            if(t < 0) {
                return a;
            }
            if(t > 1) {
                return b;
            }
            return a * (1 - t) + b * t;
        }
    
        real length() {
            auto temp = b - a;
            return temp.magnitude();
        }
    private:
        vector a;
        vector b;
    };
};

#endif
