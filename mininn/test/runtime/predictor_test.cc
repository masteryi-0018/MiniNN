#include "mininn/runtime/predictor.h"
#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/register.h"
#include "mininn/parser/parser.h"

#include "mininn/utils/log.h"
#include "mininn/utils/utils.h"

#include <gtest/gtest.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#else
#include <unistd.h>
#endif

std::shared_ptr<Graph> make_graph() {
    auto graph = std::make_shared<Graph>();
    
    // 1. set tensor first
    std::vector<int> shape = {1000, 3, 224, 224};
    void* data = nullptr;
    graph->add_tensor(shape, data);
    graph->add_tensor(shape, data);
    graph->add_tensor(shape, data);

    // 2. set node
    std::shared_ptr<Node> node = OpFactory::global().create(ADD);
    std::vector<int> inputs = {0, 1};
    std::vector<int> outputs = {2};
    std::map<std::string, std::vector<int>> attrs;
    graph->add_node(node, inputs, outputs, attrs);

    // 3. set graph input and output
    graph->set_inputs(inputs);
    graph->set_outputs(outputs);

    return graph;
}

TEST(Predictor, get_input_tensors) {
    auto graph = make_graph();

    auto predictor = std::make_shared<Predictor>(graph);
    std::vector<std::shared_ptr<Tensor>> input_tensors = predictor->get_input_tensors();
    int input_num = input_tensors.size();
    EXPECT_EQ(input_num, 2);
}

TEST(Predictor, get_output_tensors) {
    auto graph = make_graph();

    auto predictor = std::make_shared<Predictor>(graph);
    std::vector<std::shared_ptr<Tensor>> output_tensors = predictor->get_output_tensors();
    int output_num = output_tensors.size();
    EXPECT_EQ(output_num, 1);
}

TEST(Predictor, run) {
    auto graph = make_graph();

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
    float* golden = reinterpret_cast<float*>(std::malloc(size * sizeof(float)));
    for (int i = 0; i < size; ++i) {
        golden[i] = 3.0f;
    }
    bool result = is_equal(output, golden, size);
    free(golden);
    EXPECT_EQ(result, true);
}

TEST(Predictor, async_run_future) {
    auto graph = make_graph();

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
    // this is slower than async_run_future, so we should do it before async_run_future
    float* golden = reinterpret_cast<float*>(std::malloc(size * sizeof(float)));
    for (int i = 0; i < size; ++i) {
        golden[i] = 3.0f;
    }

    std::future<void> future = predictor->async_run_future();
    LOG(INFO) << "do something in main thread";
    future.get();

    std::vector<std::shared_ptr<Tensor>> output_tensors = predictor->get_output_tensors();
    float* output = reinterpret_cast<float*>(output_tensors[0]->get_buffer());

    bool result = is_equal(output, golden, size);
    free(golden);
    EXPECT_EQ(result, true);
}

TEST(Predictor, async_run_callback) {
    auto graph = make_graph();

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

    float* golden = reinterpret_cast<float*>(std::malloc(size * sizeof(float)));
    for (int i = 0; i < size; ++i) {
        golden[i] = 3.0f;
    }

    std::function<void()> callback = []() {
        LOG(INFO) << "do something in callback";
    };

    predictor->async_run_callback(callback);
    LOG(INFO) << "do something in main thread";

    sleep(3);

    std::vector<std::shared_ptr<Tensor>> output_tensors = predictor->get_output_tensors();
    float* output = reinterpret_cast<float*>(output_tensors[0]->get_buffer());
    bool result = is_equal(output, golden, size);
    free(golden);
    EXPECT_EQ(result, true);
}