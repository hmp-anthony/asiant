#include <asiant/priority_queue.hpp>
#include <asiant/graph.hpp>

#include "gtest/gtest.h"

#include <iostream>

using namespace asiant;

struct example_node_record : public node_record {
    int node_index_;
    connection connection_;
    real cost_so_far_;
};

TEST(priority_queue, push) {
    auto nr_1 = std::make_shared<example_node_record>();
    nr_1->node_index_ = 1;
    nr_1->connection_ = connection(1, 3);
    nr_1->cost_so_far_ = 0.1;

    auto nr_2 = std::make_shared<example_node_record>();
    nr_2->node_index_ = 9;
    nr_2->connection_ = connection(9, 3);
    nr_2->cost_so_far_ = 1.45;

    auto nr_3 = std::make_shared<example_node_record>();
    nr_3->node_index_ = 4;
    nr_3->connection_ = connection(4, 2);
    nr_3->cost_so_far_ = 0.45;

    auto pq = priority_queue();
    pq.push(nr_1);
    pq.push(nr_2);
    pq.push(nr_3);

    auto top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.1, 0.0001); 
}

