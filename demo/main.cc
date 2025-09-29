#include <iostream>

#include "mininn/graph/graph.h"
#include "mininn/graph/tensor.h"
#include "mininn/graph/use_kernel.h"
#include "mininn/graph/use_op.h"
#include "mininn/parser/parser.h"
#include "mininn/runtime/predictor.h"

bool is_equal(float* tensor1, float* tensor2, int size) {
  for (int i = 0; i < size; ++i) {
    if (tensor1[i] != tensor2[i]) {
      return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <model_file>" << std::endl;
    return -1;
  }
  std::string filename = argv[1];
  auto graph = std::make_shared<Graph>();
  load_model(filename, graph);

  auto predictor = std::make_shared<Predictor>(graph);
  predictor->prepare();

  std::vector<std::shared_ptr<Tensor>> input_tensors =
      predictor->get_input_tensors();
  float* input1 = reinterpret_cast<float*>(input_tensors[0]->get_buffer());
  float* input2 = reinterpret_cast<float*>(input_tensors[1]->get_buffer());
  int size = input_tensors[0]->get_size();
  for (int i = 0; i < size; ++i) {
    input1[i] = 1.0f;
    input2[i] = 2.0f;
  }

  predictor->run();

  std::vector<std::shared_ptr<Tensor>> output_tensors =
      predictor->get_output_tensors();
  float* output = reinterpret_cast<float*>(output_tensors[0]->get_buffer());
  float* golden = reinterpret_cast<float*>(std::malloc(size * sizeof(float)));
  for (int i = 0; i < size; ++i) {
    golden[i] = 3.0f;
  }
  bool result = is_equal(output, golden, size);
  free(golden);

  std::cout << std::boolalpha << result << std::endl;
  return 0;
}