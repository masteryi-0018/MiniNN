#ifndef TENSOR_H
#define TENSOR_H

#include <vector>

class Tensor {
 public:
  Tensor();

  ~Tensor();

  // todo：入参推荐用const
  void set_shape(std::vector<int>& shape);

  // 返回成员变量可以用引用，需要注意对象的生命周期
  // todo：推荐用const
  std::vector<int>& get_shape();

  int get_size();

  void* get_buffer();

  // pybind
  void set_data(std::vector<float>& data);

  // pybind
  std::vector<float> get_data();

  // flatbuffers use const void*
  void set_rawdata(const void* data);

 private:
  std::vector<int> shape_;
  void* buffer_;
  int size_;
};

#endif  // TENSOR_H