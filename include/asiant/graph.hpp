#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <asiant/precision.hpp>
#include <asiant/priority_queue.hpp>

#include <vector>
#include <memory>

namespace asiant {
    class node {
    public:
        node() {}
        node(int x, std::shared_ptr<node> t) { 
            v_ = x; next_ = t;
        }
        std::shared_ptr<node> get_next() { return next_; }
        int get_value(){ return v_; }
    private:
        int v_;
        std::shared_ptr<node> next_;
    };

    class connection {
    public:
        connection(int v = -1, int w = -1, real cost = 1) {
            v_ = v; w_ = w; cost_ = cost;
        }
        real get_cost(){ return cost_; }
        int get_from(){ return v_; }
        int get_to(){ return w_; }
    private:
        int v_, w_;
        real cost_;
    };

    struct node_record {
        node node_;
        connection connection_;
        real cost_so_far_;
        std::shared_ptr<node_record> previous_;
    };

    class graph {
    public:
        graph(int node_count, bool directed_graph = false);
        int get_node_count();
        bool is_directed();
        void insert(std::shared_ptr<connection> c);
        std::vector<std::shared_ptr<connection>> get_connections(int node_value);
    private:
        int node_count_;
        bool directed_graph_;
        std::vector<std::vector<std::shared_ptr<connection>>> connections_;
    };

    std::vector<int> dijkstra(graph g, int start, int end);
};

#endif
