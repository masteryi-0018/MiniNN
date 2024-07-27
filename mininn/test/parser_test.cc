#include "mininn/parser/parser.h"

#include <gtest/gtest.h>

TEST(Parser, load_model) {
    std::string filename = "convertor/mininn_test.gynn";
    auto graph = std::make_shared<Graph>();
    load_model(filename, graph);

    std::vector<int> inputs = {0, 1};
    std::vector<int> inputs_ = graph->get_inputs();
    EXPECT_EQ(inputs_, inputs);

    std::vector<int> outputs = {2};
    std::vector<int> outputs_ = graph->get_outputs();
    EXPECT_EQ(outputs_, outputs);

    int tensor_num = graph->get_tensors().size();
    EXPECT_EQ(tensor_num, 3);

    int node_num = graph->get_nodes().size();
    EXPECT_EQ(node_num, 1);
}