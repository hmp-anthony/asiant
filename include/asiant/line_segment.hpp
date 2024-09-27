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

        // We need to return distance from point to line_segment 
        // and the parameter value of the point on the line that
        // is closest to the point.
        struct parameter_data {
            real distance_between_line_and_point;
            real parameter;
        };
        
        parameter_data minimum_distance_to_point(const vector& p) {
            parameter_data pd;
            auto v = p - a;
            auto u = b - a;
            auto mag = u.magnitude();
            auto t = v * u;
            t = t * (1 / (mag * mag));
            if(t >= 0 && t <= 1) {
                auto distance_vector = v - u * t;
                pd.distance_between_line_and_point = distance_vector.magnitude();
                pd.parameter = t;
            } else {
                if(t > 1) {
                    auto l = p - b; 
                    pd.distance_between_line_and_point = l.magnitude();
                    pd.parameter = 1;
                } else {
                    auto l = p - a; 
                    pd.distance_between_line_and_point = l.magnitude();
                    pd.parameter = 0;   
                }
            }
            return pd;
        }
    private:
        // a is the "start" of the line segment.
        vector a;
        // b is the "end" of the line segment.
        vector b;
    };
};

#endif
