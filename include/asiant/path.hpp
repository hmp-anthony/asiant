#ifndef PATH_HPP
#define PATH_HPP

#include <asiant/line_segment.hpp>

#include <vector>
#include <iostream>
#include <memory>

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
        vector begin() {
            return line_segments[0][0];
        }
        vector end() {
            return line_segments[line_segments.size() - 1][1];
        }
        vector operator [](const real t) const {
            auto int_part = int(t);
            if(int_part >= line_segments.size()) {
                return line_segments[line_segments.size() - 1][1];
            }
            auto remainder = t - int_part;
            return line_segments[int_part][remainder];
        }
    private:
        std::vector<line_segment> line_segments; 
    };

    std::shared_ptr<path> create_path(std::vector<vector> points, bool loop);

    class path_character {
    public:
        path_character() {
            position_ = 0;
            loop_ = false;
        }
       
        void set_path(std::shared_ptr<path> path) {
            path_ = path;
            if(path->begin() == path->end()) {
                loop_ = true;
            }
        }

        void update(real velocity, real duration) {
            auto length = path_->get_length_of_current_line_segment(position_);
            auto delta = (1.0 / (real)length);
            auto factor = velocity * duration;
            position_ += delta * factor;
            auto size = path_->get_line_segments().size();
            if(position_ <= size) return;
            if(loop_) {
                position_ -= size;
            } else {
                position_ = size;
            }
        }

        vector get_position() {
            return (*path_)[position_];
        }

    protected:
        std::shared_ptr<path> path_;
        real position_;
        bool loop_;
    };

    class continuous_path_bezier {
    public:
        continuous_path_bezier(std::vector<vector> control_points)
            : control_points_(control_points) {};

        std::vector<vector> get_path() {
            std::vector<vector> path;
            auto number_of_control_points = control_points_.size();
            if(number_of_control_points < 4) {
                return path;
            }
            auto step_size = 0.01;
            auto step_count = std::size_t(1/step_size);
            auto segment_number = std::size_t(number_of_control_points / 4);
            for(std::size_t i = 0; i < segment_number; ++i) {
                auto p0 = control_points_[4 * i];
                auto p1 = control_points_[4 * i + 1];
                auto p2 = control_points_[4 * i + 2];
                auto p3 = control_points_[4 * i + 3];
                for(std::size_t k = 0; k < step_count; ++k) {
                    auto t = k * step_size;
                    auto position_on_curve = obtain_bezier_interpolation(t, p0, p1, p2, p3);
                    path.push_back(position_on_curve);
                }
            }
        }
    private:
        std::vector<vector> control_points_;

        vector obtain_bezier_interpolation(real t, vector& p0, vector& p1, vector& p2, vector& p3) {
            real b0 = std::pow(1 - t, 3);
            real b1 = 3 * t * std::pow(1 - t, 2);
            real b2 = 3 * std::pow(t, 2) * (1 - t);
            real b3 = std::pow(t, 3);
            return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
        }
    };
};

#endif
