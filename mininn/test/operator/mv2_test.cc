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

std::string mv2_filename = "models/mobilenetv2-10.gynn";

TEST(Operator_Conv, get_input_tensors) {
    auto graph = std::make_shared<Graph>();
    load_model(mv2_filename, graph);

    auto predictor = std::make_shared<Predictor>(graph);
    std::vector<std::shared_ptr<Tensor>> input_tensors = predictor->get_input_tensors();
    int input_num = input_tensors.size();
    EXPECT_EQ(input_num, 1);
}

TEST(Operator_Conv, get_output_tensors) {
    auto graph = std::make_shared<Graph>();
    load_model(mv2_filename, graph);

    auto predictor = std::make_shared<Predictor>(graph);
    std::vector<std::shared_ptr<Tensor>> output_tensors = predictor->get_output_tensors();
    int output_num = output_tensors.size();
    EXPECT_EQ(output_num, 1);
}

TEST(Operator_Conv, run) {
    auto graph = std::make_shared<Graph>();
    load_model(mv2_filename, graph);

    auto predictor = std::make_shared<Predictor>(graph);
    predictor->prepare();

    std::vector<std::shared_ptr<Tensor>> input_tensors = predictor->get_input_tensors();
    float* input1 = reinterpret_cast<float*>(input_tensors[0]->get_buffer());
    int size = input_tensors[0]->get_size();
    for (int i = 0; i < size; ++i) {
        input1[i] = 1.0f;
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