#ifndef SHAPE_H
#define SHAPE_H

#include "mininn/graph/node.h"
#include "mininn/graph/param.h"
#include "mininn/graph/type.h"

class Shape : public Node {
 public:
  Shape(Op type);

  ~Shape();

  void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_attributes(std::map<std::string, std::vector<int>>& attrs);

  void init_kernel(std::shared_ptr<Kernel> kernel);

 private:
  ShapeParams* params_;
};

#endif  // SHAPE_H