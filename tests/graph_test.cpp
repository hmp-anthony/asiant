#include <asiant/graph.hpp>

#include "gtest/gtest.h"

#include <iostream>

using namespace asiant;

TEST(graph, basic_graph_creation) {
    auto g1 = graph(10, false);
    g1.insert(graph::connection(1, 2, 0.1));
    g1.insert(graph::connection(1, 3, 0.5));
    g1.insert(graph::connection(1, 4, 1.2));
    g1.insert(graph::connection(2, 7, 0.1));
    g1.insert(graph::connection(2, 8, 0.5));
    g1.insert(graph::connection(2, 9, 1.2));
    g1.insert(graph::connection(0, 2, 1.4));
    g1.insert(graph::connection(0, 9, 2.1));
    auto connections_zero = g1.get_connections(0);
    auto connections_one = g1.get_connections(1);
    auto connections_two = g1.get_connections(2);
    ASSERT_EQ(connections_zero.size(), 2);
    ASSERT_EQ(connections_one.size(), 3);
    ASSERT_EQ(connections_two.size(), 5);

    auto g2 = graph(10, true);
    g2.insert(graph::connection(1, 2, 0.1));
    g2.insert(graph::connection(1, 3, 0.5));
    g2.insert(graph::connection(1, 4, 1.2));
    g2.insert(graph::connection(2, 7, 0.1));
    g2.insert(graph::connection(2, 8, 0.5));
    g2.insert(graph::connection(2, 9, 1.2));
    g2.insert(graph::connection(0, 2, 1.4));
    g2.insert(graph::connection(0, 9, 2.1));
    connections_zero = g2.get_connections(0);
    connections_one = g2.get_connections(1);
    connections_two = g2.get_connections(2);
    ASSERT_EQ(connections_zero.size(), 2);
    ASSERT_EQ(connections_one.size(), 3);
    ASSERT_EQ(connections_two.size(), 3);
}

TEST(graph, query_connection_vector) {
    auto g1 = graph(10, false);
    g1.insert(graph::connection(0, 2, 1.4));
    g1.insert(graph::connection(0, 9, 2.1));

    auto connections_zero = g1.get_connections(0);
    ASSERT_NEAR(connections_zero[0].cost_, 2.1, 0.00001);
    ASSERT_EQ(connections_zero[0].v_, 0);
    ASSERT_EQ(connections_zero[0].w_, 9);
    ASSERT_NEAR(connections_zero[1].cost_, 1.4, 0.00001);
    ASSERT_EQ(connections_zero[1].v_, 0);
    ASSERT_EQ(connections_zero[1].w_, 2);
}

TEST(graph, connection) {
    auto g1 = graph(10, false);
    g1.insert(graph::connection(0, 2, 1.4));
    g1.insert(graph::connection(0, 9, 2.1));

    auto connections_zero = g1.get_connections(0);
    ASSERT_NEAR(connections_zero[0].get_cost(), 2.1, 0.00001);
    ASSERT_EQ(connections_zero[0].get_from_node(), 0);
    ASSERT_EQ(connections_zero[0].get_to_node(), 9);
    ASSERT_NEAR(connections_zero[1].get_cost(), 1.4, 0.00001);
    ASSERT_EQ(connections_zero[1].get_from_node(), 0);
    ASSERT_EQ(connections_zero[1].get_to_node(), 2);
}
