#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <asiant/priority_queue.hpp>
#include <asiant/precision.hpp>

#include <memory>
#include <vector>

namespace asiant {
    // each path finding algorithm has his own node_record
    struct node_record {
        // all node_records must contain cost_so_far_
        real cost_so_far_;
    };

    class priority_queue {
    public:
        priority_queue() {};
        // inserts element and sorts underlying vector
        void push(std::shared_ptr<node_record> nr);
        // removes the top element
        int pop();
        // accesses the top element
        std::shared_ptr<node_record> top();
        // returns the number of elements
        //int size();
        // checks whether we have this node
        //bool contains(int node);
        // find and return the node_record corresponding to node
        //std::shared_ptr<node_record> find(int node);
    private:
        // used to store data
        // we use a max heap to store the node_records
        std::vector<std::shared_ptr<node_record>> heap_data_;
    };
};

#endif
