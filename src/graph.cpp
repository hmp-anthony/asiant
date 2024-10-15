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

/*    void dijkstra(graph g, int start, int goal) {
        auto start_record = std::make_shared<node_record>();
        start_record->node_ = node(start, nullptr);
        start_record->cost_so_far_ = 0;

        auto open = priority_queue();
        auto closed = priority_queue();
        open.push(start_record);
        
        auto current = std::make_shared<node_record>();

        while(open.size() > 0) {
            auto previous_current = current;
            current = open.top();
            current->previous_ = previous_current;
            std::cout << "current " << current->node_.get_value() << std::endl;
            std::cout << "current cost " << current->cost_so_far_ << std::endl;
            if(current->node_.get_value() == goal) {
                break;
            }
            auto connections = g.get_connections(current->node_.get_value());
            for(auto& connection : connections) {
                auto end_node = connection->get_to();
                auto end_node_cost = current->cost_so_far_ + connection->get_cost();
                auto end_node_record = std::make_shared<node_record>();
                if(closed.contains(end_node)) {
                    continue;
                } else if(open.contains(end_node)) {
                    auto end_node_record = open.find(end_node);
                    if(end_node_record->cost_so_far_ <= end_node_cost) continue;
                } else {
                    end_node_record->node_ = node(end_node, nullptr);
                }
                end_node_record->cost_so_far_ = end_node_cost;
                end_node_record->connection_ = *connection;
                if(!open.contains(end_node)) {
                    open.push(end_node_record);
                }
            }
            std::cout << "before pop" << std::endl;
            std::cout << "current : " << current->node_.get_value() << std::endl;
            open.print();
            open.pop();
            std::cout << "after pop" << std::endl;
            closed.push(current);
        }
        while(current != nullptr) {
            std::cout << current->node_.get_value() << std::endl;;
            current = current->previous_;
        }
    }
};*/
std::shared_ptr<node_record> dijkstra(graph g, int start, int goal) {
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
                return record;
            }
        
            // need to implement expand !
            auto connections = g.get_connections(record->node_.get_value());
            for(auto & c : connections) {
                std::cout << c->get_from() << " " << c->get_to() << std::endl;
            }
        }
    }

};
