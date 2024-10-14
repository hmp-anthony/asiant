#include <asiant/graph.hpp>

namespace asiant {
    graph::graph(int node_count, bool directed_graph) 
        : connections_(node_count),
          node_count_(node_count),
          directed_graph_(directed_graph) { };


    int graph::get_node_count() {
        return node_count_;
    }

    bool graph::is_directed() {
        return directed_graph_;
    }

    void graph::insert(std::shared_ptr<connection> c) {  
        connections_[c->get_from()].push_back(c);
        // if we have a directed graph we can end here
        if(directed_graph_) return;
        // if the graph is not directed. we need to add
        // the opposite direction.
        auto o = std::make_shared<connection>(c->get_to(), c->get_from(), c->get_cost());
        connections_[o->get_from()].push_back(o);

    }

    std::vector<std::shared_ptr<connection>> graph::get_connections(int from_node) {
        return connections_[from_node];
    }
};
