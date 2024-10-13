#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <asiant/precision.hpp>
#include <asiant/graph.hpp>

#include <memory>
#include <vector>

namespace asiant {

    struct node_record {
        node node_;
        real cost_so_far_;
    };

    class set {
    public:
        enum color { RED, BLACK };
        set() : root_(nullptr) {};
        void insert(std::shared_ptr<node_record> val);
        void remove(std::shared_ptr<node_record> val);
        //std::shared_ptr<node_record> find(real node_value);
    private:
        struct rb_node {
            std::shared_ptr<node_record> data_;
            color                    color_;
            std::shared_ptr<rb_node> left_;
            std::shared_ptr<rb_node> right_;
            std::shared_ptr<rb_node> parent_;
            explicit rb_node(std::shared_ptr<node_record> val) 
            : data_(val), color_(RED), left_(nullptr), right_(nullptr), parent_(nullptr) {};
        };
        std::shared_ptr<rb_node> root_;

        void rotate_left(std::shared_ptr<rb_node> x); 
        void rotate_right(std::shared_ptr<rb_node> x);
        void fix_insert(std::shared_ptr<rb_node> x);
        std::shared_ptr<rb_node> min_value_node(std::shared_ptr<rb_node> node);
        void transplant(std::shared_ptr<rb_node> root,
                        std::shared_ptr<rb_node> u, 
                        std::shared_ptr<rb_node> v);
        void fix_delete(std::shared_ptr<rb_node> z);
    };

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
        bool contains(real node_value);
        // find and return the node_record corresponding to node
        std::shared_ptr<node_record> find(real node_value);
    private:
        // used to store data
        // we use a min heap to store the node_records
        std::vector<std::shared_ptr<node_record>> heap_data_;
        // we use a set to implement contains and find 
        // in log time instead of linear time.
        set                                       set_data_;
    };
};

#endif
