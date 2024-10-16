#ifndef LEVEL_HPP
#define LEVEL

#include <string>
#include <vector>

namespace asiant {
    class level {
    public:
        level(std::string file, int width, int height);
       // std::shared_ptr<graph> get_graph();
       // std::vector<vector> get_node_to_vertex_map();
    private:
        std::vector<int> data_;        
    };
};

#endif
