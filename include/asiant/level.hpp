#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include <asiant/graph.hpp>

namespace asiant {
    class level {
    public:
        level(std::string file, int width, int height);
        std::shared_ptr<graph> get_graph();
        // // std::shared_ptr<std::vector<vector>> get_node_to_vertex_map();
        void print();
        int get_begin();
        int get_end();
    private:
        std::shared_ptr<graph> graph_;
        std::vector<int> data_;        
        int rows_;
        int cols_;
        int begin_;
        int end_;
    };
};

#endif
