#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <asiant/precision.hpp>
#include <asiant/graph.hpp>

#include <memory>
#include <vector>

namespace asiant {

    struct node_record;

    class priority_queue {
    public:
        priority_queue() {};
        // inserts element and sorts underlying vector
        void push(std::shared_ptr<node_record> nr);
        // removes the top element
        std::shared_ptr<node_record> pop();
        // accesses the top element
        std::shared_ptr<node_record> top();
        // returns the number of elements
        int size();
        // checks whether we have this node
        bool contains(int node_value);
        // find and return the node_record corresponding to node
        std::shared_ptr<node_record> find(int node_value);
        void print();
    private:
        // used to store data
        // we use a min heap to store the node_records
        std::vector<std::shared_ptr<node_record>> heap_data_;
    };
};

#endif
