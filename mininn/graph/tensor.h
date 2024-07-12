#ifndef TENSOR_H
#define TENSOR_H

#include <vector>

class Tensor {
public:
    Tensor();

    void set_shape(std::vector<int> shape);

    std::vector<int> get_shape();

    int get_size();

    void* get_buffer();

    int get_length();

private:
    std::vector<int> shape_;
    void* buffer_;
    int size_;
};

#endif // TENSOR_H