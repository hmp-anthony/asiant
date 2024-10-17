#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include <asiant/graph.hpp>

namespace asiant {
    class level {
    public:
        level(std::string file);
        std::shared_ptr<graph> get_graph();
        std::vector<std::pair<int, int>> get_locations();
        void print_path(std::vector<int> path);
        void print();
        int get_begin();
        int get_end();
    private:
        int rows_;
        int cols_;
        int begin_;
        int end_;
        std::shared_ptr<graph> graph_;
        std::vector<int> data_;        
        std::vector<std::pair<int, int>> locations_;
    };
};

#endif
