#include "mininn/graph/use_op.h"
#include "mininn/graph/use_kernel.h"

#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/register.h"

#include <gtest/gtest.h>

// TODO: windows can not pass this gtest
TEST(graphTest, add_node) {
    Graph* graph = new Graph();
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);
    graph->add_node(node, {0, 1}, {2});
    int node_num = graph->get_nodes().size();
    EXPECT_EQ(node_num, 1);
}

TEST(graphTest, add_tensor) {
    Graph* graph = new Graph();
    graph->add_tensor();
    int tensor_num = graph->get_tensors().size();
    EXPECT_EQ(tensor_num, 1);
}

TEST(graphTest, set_inputs) {
    Graph* graph = new Graph();
    std::vector<int> inputs = {0, 1};
    graph->set_inputs(inputs);
    std::vector<int> inputs_ = graph->get_inputs();
    EXPECT_EQ(inputs_, inputs);
}

TEST(graphTest, set_outputs) {
    Graph* graph = new Graph();
    std::vector<int> outputs = {2};
    graph->set_outputs(outputs);
    std::vector<int> outputs_ = graph->get_outputs();
    EXPECT_EQ(outputs_, outputs);
}