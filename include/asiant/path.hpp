#ifndef PATH_HPP
#define PATH_HPP

#include <asiant/line_segment.hpp>

#include <vector>

namespace asiant {
    class path {
    public:
        path() {
            smoothing = 0.99;
        }
        int path_length() {
            return line_segments.size();
        }
        void add_line_segment(line_segment ls){
            line_segments.push_back(ls);
        }
        void set_smoothing(real s) {
            smoothing = s;
        }
        std::vector<line_segment> get_line_segments() {
            return line_segments;
        }
           
        real get_parameter(const vector& v) {
            auto params = line_segments[0].minimum_distance_to_point(v);
            int i = 0;
            int index_of_closest_line_segment = 0;
            for(auto& ls : line_segments) {
                auto ps = ls.minimum_distance_to_point(v);
                if(ps.distance_between_line_and_point < params.distance_between_line_and_point) {
                    params = ps;
                    index_of_closest_line_segment = i;
                }
                i++;
            }
            auto new_param_value = params.parameter + index_of_closest_line_segment;
            auto delta = abs(new_param_value - old_param_value);
            if(delta > 0.1) {
                new_param_value -= smoothing * delta;
            }
            old_param_value = new_param_value;
            return old_param_value;
        }

        vector get_position(real param) {
            int line_segment_index = int(param);
            real line_segment_parameter = param - line_segment_index;
            return line_segments[line_segment_index][line_segment_parameter];
        }
    private:
        real old_param_value;
        real smoothing;
        std::vector<line_segment> line_segments; 
    };
};

#endif
