#include <asiant/line_segment.hpp>

#include <vector>

#include <iostream>

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
            std::cout << "------------" << std::endl;
            for(auto& ls : line_segments) {
                auto ps = ls.minimum_distance_to_point(v);
                std::cout << "integer " << i << std::endl;
                std::cout << "parameter " << ps.parameter << std::endl;
                std::cout << "distance " << ps.distance_between_line_and_point << std::endl;
                if(ps.distance_between_line_and_point < params.distance_between_line_and_point) {
                    params = ps;
                    index_of_closest_line_segment = i;
                }
                i++;
            }
            std::cout << "------------" << std::endl;
            std::cout << index_of_closest_line_segment << std::endl;
            std::cout << params.distance_between_line_and_point << std::endl;
            std::cout << params.parameter << std::endl;
            return 0;
        }
    private:
        std::vector<line_segment> line_segments; 
    };
};


