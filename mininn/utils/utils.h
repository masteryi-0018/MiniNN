#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#else
#include <unistd.h>
#endif

bool is_equal(float* tensor1, float* tensor2, int size);

double l2_norm(float* tensor1, float* tensor2, int size);

std::vector<float> load_txt(const std::string& filename);

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