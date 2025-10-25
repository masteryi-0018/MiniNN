#include <fstream>

#include "mininn/utils/utils.h"

bool is_equal(float* tensor1, float* tensor2, int size) {
  for (int i = 0; i < size; ++i) {
    if (tensor1[i] != tensor2[i]) {
      return false;
    }
  }
  return true;
}

double l2_norm(float* tensor1, float* tensor2, int size) {
  double sum = 0.0;
  for (int i = 0; i < size; ++i) {
      double diff = tensor1[i] - tensor2[i];
      sum += diff * diff;
  }
  return std::sqrt(sum);
}

std::vector<float> load_txt(const std::string& filename) {
    std::vector<float> result;
    std::ifstream file(filename);
    float value;
    while (file >> value) {
        result.push_back(value);
    }
    return result;
}