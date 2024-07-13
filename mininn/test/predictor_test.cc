#include "mininn/graph/use_op.h"
#include "mininn/graph/use_kernel.h"

#include "mininn/runtime/predictor.h"
#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/register.h"

#include <gtest/gtest.h>

bool is_equal(float* tensor1, float* tensor2, int size) {
    for (int i = 0; i < size; ++i) {
        if (tensor1[i] != tensor2[i]) {
            return false;
        }
    }
    return true;
}

TEST(graphTest, prepare) {
    auto graph = std::make_shared<Graph>();
    
    // 1. set tensor first
    std::vector<int> shape = {1, 3, 224, 224};
    graph->add_tensor(shape);
    graph->add_tensor(shape);
    graph->add_tensor(shape);

    // 2. set node
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    graph->add_node(node, inputs, outputs);

    // 3. set graph input and output
    graph->set_inputs(inputs);
    graph->set_outputs(outputs);

    auto predictor = std::make_shared<Predictor>(graph);
    predictor->prepare();
    int kernel_num = predictor->get_kernels().size();
    EXPECT_EQ(kernel_num, 1);
}

TEST(graphTest, run) {
    auto graph = std::make_shared<Graph>();
    
    // 1. set tensor first
    std::vector<int> shape = {1, 3, 224, 224};
    graph->add_tensor(shape);
    graph->add_tensor(shape);
    graph->add_tensor(shape);

    // 2. set node
    std::shared_ptr<Node> node = OpFactory::Global().Create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    graph->add_node(node, inputs, outputs);

    // 3. set graph input and output
    graph->set_inputs(inputs);
    graph->set_outputs(outputs);

    auto predictor = std::make_shared<Predictor>(graph);
    predictor->prepare();

    std::vector<std::shared_ptr<Tensor>> input_tensors = predictor->get_input_tensors();
    float* input1 = reinterpret_cast<float*>(input_tensors[0]->get_buffer());
    float* input2 = reinterpret_cast<float*>(input_tensors[1]->get_buffer());
    int size = input_tensors[0]->get_size();
    for (int i = 0; i < size; ++i) {
        input1[i] = 1.0f;
        input2[i] = 2.0f;
    }

    predictor->run();

    std::vector<std::shared_ptr<Tensor>> output_tensors = predictor->get_output_tensors();
    float* output = reinterpret_cast<float*>(output_tensors[0]->get_buffer());
    float* golden = reinterpret_cast<float*>(std::malloc(size));
    for (int i = 0; i < size; ++i) {
        golden[i] = 3.0f;
    }
    bool result = is_equal(output, golden, size);
    EXPECT_EQ(result, true);
}