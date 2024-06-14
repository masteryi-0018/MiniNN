#include "use_op.h"

#include "graph.h"
#include "node.h"
#include "register.h"
#include "type.h"

#include <gtest/gtest.h>

// TODO: windows can not pass this gtest
TEST(graphTest, add_node) {
    Graph* graph = new Graph();
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);

    graph->add_node(node, {0, 1}, {2});
    int size = graph->get_nodes().size();
    EXPECT_EQ(size, 1);
}

TEST(graphTest, add_tensor) {
    Graph* graph = new Graph();

    std::vector<int> shape = {1, 3, 224, 224};
    int size = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    std::vector<float> data;
    data.resize(size);
    for (int i = 0; i < size; ++i) {
        data[i] = 1.0f;
    }
    Tensor* tensor = new Tensor(shape, data);

    graph->add_tensor(tensor);
    int tensor_num = graph->get_tensors().size();
    EXPECT_EQ(tensor_num, 1);
}

TEST(graphTest, set_inputs) {
    Graph* graph = new Graph();
    std::vector<int> inputs = {0, 1};
    graph->set_inputs(inputs);
    std::vector<int> inputs_ = graph->get_inputs();
    EXPECT_EQ(inputs, inputs_);
}

TEST(graphTest, set_outputs) {
    Graph* graph = new Graph();
    std::vector<int> outputs = {2};
    graph->set_outputs(outputs);
    std::vector<int> outputs_ = graph->get_outputs();
    EXPECT_EQ(outputs, outputs_);
}