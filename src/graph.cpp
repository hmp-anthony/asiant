#include <asiant/graph.hpp>

namespace asiant {
    graph::graph(int vertex_count, bool directed_graph) : adj_list_(vertex_count),
                                                          vertex_count_(vertex_count),
                                                          edge_count_(0),
                                                          directed_graph_(directed_graph) {
                                                          adj_list.assign(vertex_count, nullptr)    
                                                          }


    int graph::get_vertex_count() {
        return vertex_count_;
    }

    int graph::get_connection_count() {
        return connection_count_;
    }

    bool graph::is_directed() {
        return is_directed_;
    }

    void insert(connection c) {
        int v = c.v_, w = c.w_, cost = c.cost_;
        auto n = std::make_shared<node>(w, adj_list[v], cost);
        adj_list[v] = n;
        if(!directed_graph_) {
            auto m = std::make_shared<node>(v, adj_list[w], cost);
            adj_list[w] = m;
        }
    }

    std::vector<connection> get_connections(int from_node) {
        auto t = adj_list_[from_node];

    }
};
