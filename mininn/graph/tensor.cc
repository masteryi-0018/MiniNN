#include "tensor.h"

Tensor::Tensor() {}

Tensor::Tensor(const std::vector<int>& shape) {
    shape_ = shape;
    size_ = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    data_.resize(size_);
}

Tensor::Tensor(const std::vector<int>& shape, const std::vector<float>& data) {
    shape_ = shape;
    size_ = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
    data_ = data;
}

Tensor Tensor::operator+(const Tensor& other) const {
    check_shape(other.shape_);
    Tensor result(shape_);
    std::transform(data_.begin(), data_.end(), other.data_.begin(), result.data_.begin(), std::plus<float>());
    return result;
}

Tensor Tensor::operator-(const Tensor& other) const {
    check_shape(other.shape_);
    Tensor result(shape_);
    std::transform(data_.begin(), data_.end(), other.data_.begin(), result.data_.begin(), std::minus<float>());
    return result;
}

Tensor Tensor::operator*(const Tensor& other) const {
    check_shape(other.shape_);
    Tensor result(shape_);
    std::transform(data_.begin(), data_.end(), other.data_.begin(), result.data_.begin(), std::multiplies<float>());
    return result;
}

Tensor Tensor::operator/(const Tensor& other) const {
    check_shape(other.shape_);
    Tensor result(shape_);
    std::transform(data_.begin(), data_.end(), other.data_.begin(), result.data_.begin(), std::divides<float>());
    return result;
}

std::vector<int> Tensor::shape() const {
    return shape_;
}

int Tensor::size() const {
    return size_;
}

std::vector<float> Tensor::data() const {
    return data_;
}

void Tensor::check_shape(const std::vector<int>& other_shape) const {
    if (shape_ != other_shape) {
        throw std::invalid_argument("Invalid shapes for element-wise operation");
    }
}