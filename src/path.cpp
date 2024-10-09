#include <asiant/path.hpp>

namespace asiant {

    std::shared_ptr<path> create_path(std::vector<vector> points, bool loop) {
        auto p = std::make_shared<path>(); 
        for(std::size_t i = 0; i < points.size() - 1; ++i) {
            line_segment l(points[i], points[i + 1]);
            p->add_line_segment(l);
        }
        if(loop) {
            line_segment l(points[points.size() - 1], points[0]);
            p->add_line_segment(l);
        }
        return p;
    }
};
