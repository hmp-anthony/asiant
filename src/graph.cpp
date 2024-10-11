#include <asiant/graph.hpp>

namespace asiant {
    graph::graph(int vertex_count, bool directed_graph) : adj_list_(vertex_count),
                                                          vertex_count_(vertex_count),
                                                          connection_count_(0),
                                                          directed_graph_(directed_graph) {
                                                          adj_list_.assign(vertex_count, nullptr);
                                                          };


    int graph::get_vertex_count() {
        return vertex_count_;
    }

    int graph::get_connection_count() {
        return connection_count_;
    }

    bool graph::is_directed() {
        return directed_graph_;
    }

#include<iostream>

    void graph::insert(connection c) {
        int v = c.v_, w = c.w_;
        real cost = c.cost_;
        adj_list_[v] = std::make_shared<node>(w, adj_list_[v], cost);
        if(!directed_graph_) {
            adj_list_[w] = std::make_shared<node>(v, adj_list_[w], cost);
        }
    }

    std::vector<graph::connection> graph::get_connections(int from_node) {
        auto connection_vector = std::vector<connection>();
        auto t = adj_list_[from_node];
        //std::cout << t->v_ << std::endl;
        if(t == nullptr){ return connection_vector; }
        do {
            auto c = connection(from_node, t->v_, t->cost_);
            connection_vector.push_back(c);
            t = t->next_;
        } while(t != nullptr) ;
    }
};
