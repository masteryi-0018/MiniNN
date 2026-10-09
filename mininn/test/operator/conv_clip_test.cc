#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <vector>

#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/register.h"
#include "mininn/graph/type.h"
#include "mininn/runtime/predictor.h"

namespace {

// Conv has optional min/max attrs for a fused clip.
//
// fixed case: input 1x1x4x4 all ones, weight 1x1x3x3 all ones, bias 0, pads 1
// conv output (sum of the 3x3 window at each position):
//   4 6 6 4
//   6 9 9 6
//   6 9 9 6
//   4 6 6 4
std::vector<float> run_conv(bool with_clip) {
  auto graph = std::make_shared<Graph>();

  std::vector<int> input_shape = {1, 1, 4, 4};
  std::vector<int> weight_shape = {1, 1, 3, 3};
  std::vector<int> bias_shape = {1};
  std::vector<int> out_shape = {1, 1, 4, 4};
  std::vector<float> input_data(16, 1.0f);
  std::vector<float> weight_data(9, 1.0f);
  std::vector<float> bias_data(1, 0.0f);

  // 1. set tensors first
  graph->add_tensor(input_shape, input_data.data());
  graph->add_tensor(weight_shape, weight_data.data());
  graph->add_tensor(bias_shape, bias_data.data());
  graph->add_tensor(out_shape, nullptr);

  // 2. set node
  std::shared_ptr<Node> node = OpFactory::global().create(CONV);
  EXPECT_NE(node, nullptr);
  std::vector<int> inputs = {0, 1, 2};
  std::vector<int> outputs = {3};
  std::map<std::string, std::vector<int>> attrs;
  attrs["dilations"] = {1, 1};
  attrs["group"] = {1};
  attrs["kernel_shape"] = {3, 3};
  attrs["pads"] = {1, 1, 1, 1};
  attrs["strides"] = {1, 1};
  if (with_clip) {
    attrs["min"] = {5};
    attrs["max"] = {7};
  }
  graph->add_node(node, inputs, outputs, attrs);

  // 3. set graph input and output
  std::vector<int> graph_inputs = {0};
  std::vector<int> graph_outputs = {3};
  graph->set_inputs(graph_inputs);
  graph->set_outputs(graph_outputs);

  auto predictor = std::make_shared<Predictor>(graph);
  predictor->prepare();
  predictor->run();

  std::vector<std::shared_ptr<Tensor>> output_tensors =
      predictor->get_output_tensors();
  float* out = reinterpret_cast<float*>(output_tensors[0]->get_buffer());
  return std::vector<float>(out, out + 16);
}

}  // namespace

// without min/max the fused clip is disabled
TEST(Operator_Conv_Clip, run_without_clip) {
  std::vector<float> expected = {4, 6, 6, 4, 6, 9, 9, 6,
                                 6, 9, 9, 6, 4, 6, 6, 4};
  std::vector<float> out = run_conv(false);
  for (int i = 0; i < 16; ++i) {
    EXPECT_FLOAT_EQ(out[i], expected[i]);
  }
}

// with min/max the output is clipped, equals Conv followed by Clip
TEST(Operator_Conv_Clip, run_with_clip) {
  std::vector<float> expected = {5, 6, 6, 5, 6, 7, 7, 6,
                                 6, 7, 7, 6, 5, 6, 6, 5};
  std::vector<float> out = run_conv(true);
  for (int i = 0; i < 16; ++i) {
    EXPECT_FLOAT_EQ(out[i], expected[i]);
  }
}
