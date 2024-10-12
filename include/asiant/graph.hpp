#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <asiant/asiant.hpp>

#include <vector>

namespace asiant {
    class node {
    public:
        node(int x, std::shared_ptr<node> t, real cost) { 
            v_ = x; next_ = t; cost_ = cost;
        }
        std::shared_ptr<node> get_next() { return next_; }
        int get_value(){ return v_; }
        real get_cost(){ return cost_; }
    private:
        int v_;
        real cost_;
        std::shared_ptr<node> next_;
    };

    class connection {
    public:
        connection(int v = -1, int w = -1, real cost = 0) {
            v_ = v; w_ = w; cost_ = cost;
        }
        real get_cost(){ return cost_; }
        int get_from(){ return v_; }
        int get_to(){ return w_; }
    private:
        int v_, w_;
        real cost_;
    };

    class graph {
    public:
        graph(int vertex_count, bool directed_graph = false);
        int get_vertex_count();
        int get_connection_count();
        bool is_directed();
        void insert(connection c);
        std::vector<connection> get_connections(int from_node);
    private:
        int vertex_count_;
        int connection_count_;
        bool directed_graph_;
        std::vector<std::shared_ptr<node>> adj_list_;
    };
};

#endif
