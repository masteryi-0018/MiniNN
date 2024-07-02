#include "mininn/graph/use_op.h"
#include "mininn/graph/use_kernel.h"

#include "mininn/runtime/predictor.h"
#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/register.h"

#include <gtest/gtest.h>

TEST(graphTest, prepare) {
    auto graph = std::make_shared<Graph>();
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    graph->add_node(node, inputs, outputs);

    graph->add_tensor();
    graph->add_tensor();
    graph->add_tensor();

    graph->set_inputs(inputs);
    graph->set_outputs(outputs);

    auto predictor = std::make_shared<Predictor>(graph);
    predictor->prepare();
    int kernel_num = predictor->get_kernels().size();
    EXPECT_EQ(kernel_num, 1);
}

TEST(graphTest, run) {
    auto graph = std::make_shared<Graph>();
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    graph->add_node(node, inputs, outputs);

    graph->add_tensor();
    graph->add_tensor();
    graph->add_tensor();

    graph->set_inputs(inputs);
    graph->set_outputs(outputs);

    auto predictor = std::make_shared<Predictor>(graph);
    predictor->prepare();
    predictor->run();
}