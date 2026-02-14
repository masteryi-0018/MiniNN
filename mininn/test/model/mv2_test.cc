#include <gtest/gtest.h>

#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/register.h"
#include "mininn/graph/type.h"
#include "mininn/parser/parser.h"
#include "mininn/runtime/predictor.h"
#include "mininn/utils/log.h"
#include "mininn/utils/utils.h"

std::string mv2_filename = "models/mobilenetv2-10.gynn";

TEST(Operator_Conv, get_input_tensors) {
  auto graph = std::make_shared<Graph>();
  load_model(mv2_filename, graph);

  auto predictor = std::make_shared<Predictor>(graph);
  std::vector<std::shared_ptr<Tensor>> input_tensors =
      predictor->get_input_tensors();
  int input_num = static_cast<int>(input_tensors.size());
  EXPECT_EQ(input_num, 1);
}

TEST(Operator_Conv, get_output_tensors) {
  auto graph = std::make_shared<Graph>();
  load_model(mv2_filename, graph);

  auto predictor = std::make_shared<Predictor>(graph);
  std::vector<std::shared_ptr<Tensor>> output_tensors =
      predictor->get_output_tensors();
  int output_num = static_cast<int>(output_tensors.size());
  EXPECT_EQ(output_num, 1);
}

TEST(Operator_Conv, run) {
  auto graph = std::make_shared<Graph>();
  load_model(mv2_filename, graph);

  auto predictor = std::make_shared<Predictor>(graph);
  predictor->prepare();

  std::vector<std::shared_ptr<Tensor>> input_tensors =
      predictor->get_input_tensors();
  float* input1 = reinterpret_cast<float*>(input_tensors[0]->get_buffer());
  int input_size = input_tensors[0]->get_size();

  std::vector<float> input = load_txt("./models/mv2_input.txt");
  for (int i = 0; i < input_size; ++i) {
    input1[i] = input[i];
  }

  predictor->run();

  std::vector<std::shared_ptr<Tensor>> output_tensors =
      predictor->get_output_tensors();
  float* output = reinterpret_cast<float*>(output_tensors[0]->get_buffer());

  int out_size = output_tensors[0]->get_size();
  std::vector<float> golden = load_txt("./models/mv2_golden.txt");

  double result = l2_norm(output, golden.data(), out_size);
  printf("L2 norm: %f\n", result);
  EXPECT_LT(result, 1e-3);
}