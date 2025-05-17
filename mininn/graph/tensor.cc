#include "mininn/graph/tensor.h"

#include <cstdlib> // std::malloc
#include <numeric> // std::accumulate
#include <functional> // std::multiplies, bazel needs
#include <cstring> // std::memcpy
#include <iostream> // std::cerr

Tensor::Tensor() {
    buffer_ = nullptr;
    size_ = 0;
}

Tensor::~Tensor() {
    if (buffer_ != nullptr) {
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

void Tensor::set_data(std::vector<float>& data) {
    if ((int)data.size() != size_) {
        std::cerr << "Data size mismatch" << std::endl;
        return;
    }
    std::memcpy(buffer_, data.data(), size_ * sizeof(float));
}

// 返回局部变量不可以使用引用，否则会造成悬空引用
std::vector<float> Tensor::get_data() {
    std::vector<float> data(size_);
    std::memcpy(data.data(), buffer_, size_ * sizeof(float));
    return data;
}

void Tensor::set_rawdata(const void* data) {
    if (data != nullptr) {
        std::memcpy(buffer_, data, size_ * sizeof(float));
    }
}