#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <asiant/asiant.hpp>

#include <vector>


namespace asiant {
    class graph {
    private:
        struct node;
    public:
        struct connection {
            int v_, w_;
            real cost_;
            connection(int v = -1, int w = -1, real cost = 0) {
                v_ = v; w_ = w; cost_ = cost;
            }
            real get_cost(){ return cost_; }
            int get_from_node(){ return v_; }
            int get_to_node(){ return w_; }
        };

        graph(int vertex_count, bool directed_graph = false);
        int get_vertex_count();
        int get_connection_count();
        bool is_directed();
        void insert(connection c);
        std::vector<connection> get_connections(int from_node);
    private:
        struct node {
            int v_;
            real cost_;
            std::shared_ptr<node> next_;
            node(int x, std::shared_ptr<node> t, real cost) { 
                v_ = x; next_ = t; cost_ = cost;
            }
        };
        typedef std::shared_ptr<node> link;
    
        int vertex_count_;
        int connection_count_;
        bool directed_graph_;
        std::vector<link> adj_list_;
    };
};

#endif
