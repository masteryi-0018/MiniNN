#include "mininn/graph/tensor.h"

#include <cstdlib> // std::malloc
#include <numeric> // std::accumulate
#include <functional> // std::multiplies, bazel needs

Tensor::Tensor() {
    buffer_ = nullptr;
    size_ = 0;
}

void Tensor::set_shape(std::vector<int>& shape) {
    shape_ = shape;
    size_ = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    // todo: find the ptr and heap malloc order
    buffer_ = std::malloc(size_ * sizeof(float));
}

std::vector<int>& Tensor::get_shape() {
    return shape_;
}

int Tensor::get_size() {
    return size_;
}

void* Tensor::get_buffer() {
    return buffer_;
}

int Tensor::get_length() {
    return size_ * sizeof(float);
}