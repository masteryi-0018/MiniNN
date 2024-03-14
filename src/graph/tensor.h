#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

class Tensor {
public:
    Tensor(const std::vector<int>& shape);

    Tensor(const std::vector<int>& shape, const std::vector<float>& data);

    Tensor operator+(const Tensor& other) const;

    Tensor operator-(const Tensor& other) const;

    // 点积
    Tensor operator*(const Tensor& other) const;

    Tensor operator/(const Tensor& other) const;

    std::vector<int> shape() const;

    int size() const;

    std::vector<float> data() const;

private:
    std::vector<int> shape_;
    int size_;
    std::vector<float> data_;

    void check_shape(const std::vector<int>& other_shape) const;
};