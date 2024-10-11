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
    std::cout << connections_zero.size() << std::endl; 
    std::cout << connections_one.size() << std::endl; 
    std::cout << connections_two.size() << std::endl; 

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
    std::cout << connections_zero.size() << std::endl; 
    std::cout << connections_one.size() << std::endl; 
    std::cout << connections_two.size() << std::endl; 
}
