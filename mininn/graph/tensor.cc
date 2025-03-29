#include "mininn/graph/tensor.h"

#include <cstdlib> // std::malloc
#include <numeric> // std::accumulate
#include <functional> // std::multiplies, bazel needs
#include <cstring> // std::memcpy

Tensor::Tensor() {
    buffer_ = nullptr;
    size_ = 0;
}

Tensor::~Tensor() {
    if (buffer_) {
        free(buffer_);
        buffer_ = nullptr;
    }
}

void Tensor::set_shape(std::vector<int>& shape) {
    if (shape.data() == nullptr) {
        // tensor: [1, 3, 224, 224]
        // number: [1]
        shape_ = std::vector<int>(1, 1); // 对待空shape，初始化为一个1
        size_ = 1;
    } else {
        shape_ = shape;
        size_ = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    }
    // in Linux, heap usually grow to low address memory
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

// pybind
void Tensor::set_data(std::vector<float>& data) {
    if (data.size() != size_) {
        throw std::runtime_error("Data size mismatch");
    }
    std::memcpy(buffer_, data.data(), size_ * sizeof(float));
}

// pybind
std::vector<float> Tensor::get_data() {
    std::vector<float> data(size_);
    std::memcpy(data.data(), buffer_, size_ * sizeof(float));
    return data;
}