#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/register.h"

#include <gtest/gtest.h>

// todo: windows can not pass this gtest
TEST(Graph, add_node) {
    // you should add_tensor before add_node
    auto graph = std::make_shared<Graph>();
    std::vector<int> shape = {1, 3, 224, 224};
    graph->add_tensor(shape);
    graph->add_tensor(shape);
    graph->add_tensor(shape);
    int tensor_num = graph->get_tensors().size();
    EXPECT_EQ(tensor_num, 3);

    std::shared_ptr<Node> node = OpFactory::global().create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    std::map<std::string, std::vector<int>> attrs;
    graph->add_node(node, inputs, outputs, attrs);
    int node_num = graph->get_nodes().size();
    EXPECT_EQ(node_num, 1);
}

TEST(Graph, add_tensor) {
    auto graph = std::make_shared<Graph>();
    std::vector<int> shape = {1, 3, 224, 224};
    graph->add_tensor(shape);
    int tensor_num = graph->get_tensors().size();
    EXPECT_EQ(tensor_num, 1);
}

TEST(Graph, set_inputs) {
    auto graph = std::make_shared<Graph>();
    std::vector<int> inputs = {0, 1};
    graph->set_inputs(inputs);
    std::vector<int> inputs_ = graph->get_inputs();
    EXPECT_EQ(inputs_, inputs);
}

TEST(Graph, set_outputs) {
    auto graph = std::make_shared<Graph>();
    std::vector<int> outputs = {2};
    graph->set_outputs(outputs);
    std::vector<int> outputs_ = graph->get_outputs();
    EXPECT_EQ(outputs_, outputs);
}