#ifndef PATH_HPP
#define PATH_HPP

#include <asiant/line_segment.hpp>

#include <vector>
#include <iostream>

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
        
        real get_length_of_current_line_segment(real position) {
            auto line_segment = line_segments[int(position)];
            return line_segment.length();
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
            position_ = 0;
        }
       
        void set_path(path path) {
            path_ = path;
        }

        void update(real velocity, real duration) {
            auto length = path_.get_length_of_current_line_segment(position_);
            auto delta = (1.0 / (real)length);
            auto factor = velocity * duration;
            position_ += delta * factor;
            auto size = path_.get_line_segments().size();
            if(position_ > size) {
                position_ -= size;
            }
        }

        vector get_position() {
            return path_[position_];
        }
    private:
        path path_;
        real position_;
    };
};

#endif
