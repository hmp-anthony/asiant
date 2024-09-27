#ifndef PATH_HPP
#define PATH_HPP

#include <asiant/line_segment.hpp>

#include <vector>

namespace asiant {
    class path {
    public:
        int path_length() {
            return line_segments.size();
        }
        void add_line_segment(line_segment ls){
            line_segments.push_back(ls);
        }
        std::vector<line_segment>& get_line_segments() {
            return line_segments;
        }

        vector operator [](const real t) const {
            auto int_part = int(t);
            auto remainder = t - int_part;
            return line_segments[int_part][remainder];
        }
    private:
        std::vector<line_segment> line_segments; 
    };

    class path_constrained_entity {
    public:
        path_constrained_entity() {
            position_parameter = 0;
        }
       
        void set_path(path pth) {
            p = pth;
        }

        void update() {
            position_parameter += 0.01;
            auto size = p.get_line_segments().size();
            if(position_parameter > size) {
                position_parameter -= size;
            }
        }

        vector get_position() {
            return p[position_parameter];
        }
    private:
        path p;
        real position_parameter;
    };
};

#endif
