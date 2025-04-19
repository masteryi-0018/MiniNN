#ifndef TENSOR_H
#define TENSOR_H

#include <vector>

class Tensor {
public:
    Tensor();

    ~Tensor();

    void set_shape(std::vector<int>& shape);

    std::vector<int>& get_shape();

    // 这里使用int是因为int最大为2,147,483,647，取10 0000 0000为例
    // 这个长度对应内存字节为40 0000 0000，也就是4G，所以在size溢出前内存已经超了
    int get_size();

    void* get_buffer();

    int get_length();

    // pybind
    void set_data(std::vector<float>& data);

    // pybind
    std::vector<float> get_data();

    void set_rawdata(const void* data);

private:
    std::vector<int> shape_;
    void* buffer_;
    int size_;
};

#endif // TENSOR_H