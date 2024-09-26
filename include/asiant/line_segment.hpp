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

        // Use this when you are certain that no OOB access will happen.
        // Or you might want this behaviour. 
        vector operator [](const real t) const {
            return a * (1 - t) + b * t;
        }

        // Use this when you are NOT certain OOB access will happen.
        vector at(const real t) const {
            if(t < 0) {
                return a;
            }
            if(t > 1) {
                return b;
            }
            return a * (1 - t) + b * t;
        }
    private:
        // a is the "start" of the line segment.
        vector a;
        // b is the "end" of the line segment.
        vector b;
    };
};

#endif
