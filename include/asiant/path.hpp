#include <asiant/line_segment.hpp>

#include <vector>

namespace asiant {
    class path {
    public:
        void add_line_segment(line_segment ls){
            line_segments.push_back(ls);
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
            return params.parameter + index_of_closest_line_segment;
        }
    private:
        std::vector<line_segment> line_segments; 
    };
};


