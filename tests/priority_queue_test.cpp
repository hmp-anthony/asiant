#include <asiant/priority_queue.hpp>
#include <asiant/graph.hpp>

#include "gtest/gtest.h"

#include <iostream>

using namespace asiant;

TEST(priority_queue, push) {
    auto nr_1 = std::make_shared<node_record>();
    nr_1->node_ = node(1, nullptr);
    nr_1->cost_so_far_ = 0.9;
    auto nr_2 = std::make_shared<node_record>();
    nr_2->node_ = node(9, nullptr);
    nr_2->cost_so_far_ = 1.45;

    auto nr_3 = std::make_shared<node_record>();
    nr_3->node_ = node(4, nullptr);
    nr_3->cost_so_far_ = 0.45;

    auto nr_4 = std::make_shared<node_record>();
    nr_4->node_ = node(4, nullptr);
    nr_4->cost_so_far_ = 0.59;

    auto pq = priority_queue();
    pq.push(nr_1);
    pq.push(nr_2);
    pq.push(nr_3);
    pq.push(nr_4);

    auto top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.45, 0.0001); 
}

#include <iostream>
TEST(priority_queue, pop) {
    auto nr_1 = std::make_shared<node_record>();
    nr_1->node_ = node(1, nullptr);
    nr_1->cost_so_far_ = 0.9;

    auto nr_2 = std::make_shared<node_record>();
    nr_2->node_ = node(2, nullptr);
    nr_2->cost_so_far_ = 1.45;

    auto nr_3 = std::make_shared<node_record>();
    nr_3->node_ = node(4, nullptr);
    nr_3->cost_so_far_ = 0.45;

    auto nr_4 = std::make_shared<node_record>();
    nr_4->node_ = node(6, nullptr);
    nr_4->cost_so_far_ = 0.59;

    auto nr_5 = std::make_shared<node_record>();
    nr_5->node_ = node(8, nullptr);
    nr_5->cost_so_far_ = 1.9;

    auto nr_6 = std::make_shared<node_record>();
    nr_6->node_ = node(9, nullptr);
    nr_6->cost_so_far_ = 2.45;

    auto nr_7 = std::make_shared<node_record>();
    nr_7->node_ = node(11, nullptr);
    nr_7->cost_so_far_ = 3.45;

    auto nr_8 = std::make_shared<node_record>();
    nr_8->node_ = node(3, nullptr);
    nr_8->cost_so_far_ = 4.59;

    auto nr_9 = std::make_shared<node_record>();
    nr_9->node_ = node(5, nullptr);
    nr_9->cost_so_far_ = 14.59;

    auto nr_10 = std::make_shared<node_record>();
    nr_10->node_ = node(19, nullptr);
    nr_10->cost_so_far_ = 0.09;

    auto nr_11 = std::make_shared<node_record>();
    nr_11->node_ = node(20, nullptr);
    nr_11->cost_so_far_ = 0.059;

    auto pq = priority_queue();
    pq.push(nr_1);
    pq.push(nr_2);
    pq.push(nr_3);
    pq.push(nr_4);
    pq.push(nr_5);
    pq.push(nr_6);
    pq.push(nr_7);
    pq.push(nr_8);
    pq.push(nr_9);
    pq.push(nr_10);
    pq.push(nr_11);

    auto top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.059, 0.0001); 

    pq.pop(); 
    top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.09, 0.0001); 

    pq.pop();
    top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.45, 0.0001); 

    pq.pop();
    top = pq.top();
    ASSERT_NEAR(top->cost_so_far_, 0.59, 0.0001); 
}

TEST(priority_queue, find_and_contains) {
    auto nr_1 = std::make_shared<node_record>();
    nr_1->node_ = node(1, nullptr);
    nr_1->cost_so_far_ = 0.9;

    auto nr_2 = std::make_shared<node_record>();
    nr_2->node_ = node(2, nullptr);
    nr_2->cost_so_far_ = 1.45;

    auto nr_3 = std::make_shared<node_record>();
    nr_3->node_ = node(4, nullptr);
    nr_3->cost_so_far_ = 0.45;

    auto nr_4 = std::make_shared<node_record>();
    nr_4->node_ = node(6, nullptr);
    nr_4->cost_so_far_ = 0.59;

    auto pq = priority_queue();
    pq.push(nr_1);
    pq.push(nr_2);
    pq.push(nr_3);
    pq.push(nr_4);

    bool find_1 = pq.find(6) != nullptr;
    ASSERT_TRUE(find_1);

    bool find_2 = pq.find(9) == nullptr;
    ASSERT_TRUE(find_2);

    ASSERT_FALSE(pq.contains(7));
    ASSERT_TRUE(pq.contains(1));

    auto find_output = pq.find(6);
    ASSERT_EQ(find_output->node_.get_value(), 6);
}
