#include <asiant/graph.hpp>

#include <algorithm>

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


    std::vector<int> dijkstra(graph g, int start, int goal) {
        std::vector<int> path;

        auto record = std::make_shared<node_record>();
        record->node_ = node(start, nullptr);
        record->cost_so_far_ = 0;

        auto frontier = priority_queue();
        frontier.push(record);

        auto reached = std::vector<std::shared_ptr<node_record>>(g.get_node_count());
        reached[start] = record;

        while(frontier.size() > 0) {
            record = frontier.top(); frontier.pop();
            if(record->node_.get_value() == goal) {
                while(record != nullptr) {
                    path.push_back(record->node_.get_value());
                    record = record->previous_;
                }
                std::ranges::reverse(path);
                return path;
            }
        
            // from these connections we need to form children
            auto connections = g.get_connections(record->node_.get_value());

            std::vector<std::shared_ptr<node_record>> children(connections.size());
            for(int i = 0; i < children.size(); ++i) {
                children[i] = std::make_shared<node_record>();
                children[i]->node_ = node(connections[i]->get_to(), nullptr);
                children[i]->previous_ = record;
                children[i]->connection_ = *(connections[i]);
                children[i]->cost_so_far_ = record->cost_so_far_ + connections[i]->get_cost();
            }
            for(auto & child: children) {
                auto s = child->node_.get_value();
                if(reached[s] == nullptr || child->cost_so_far_ < reached[s]->cost_so_far_) {
                    reached[s] = child;
                    frontier.push(child);
                }
            }

        }
        return path;
    }

};
