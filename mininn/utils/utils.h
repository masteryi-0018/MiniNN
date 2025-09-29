#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

bool is_equal(float* tensor1, float* tensor2, int size);

template <typename T>
void print_vector(const std::vector<T>& vec) {
  std::cout << "[";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i];
    if (i != vec.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

#endif  // UTILS_H