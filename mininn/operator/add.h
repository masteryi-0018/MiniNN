#ifndef ADD_H
#define ADD_H

#include "mininn/graph/node.h"
#include "mininn/graph/param.h"
#include "mininn/graph/type.h"

class Add : public Node {
 public:
  Add(Op type);

  ~Add();

  void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_attributes(std::map<std::string, std::vector<int>>& attrs);

  void init_kernel(std::shared_ptr<Kernel> kernel);

 private:
  AddParams* params_;
};

#endif  // ADD_H