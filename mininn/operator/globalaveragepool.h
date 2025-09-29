#ifndef GLOBALAVERAGEPOOL_H
#define GLOBALAVERAGEPOOL_H

#include "mininn/graph/node.h"
#include "mininn/graph/param.h"
#include "mininn/graph/type.h"

class Globalaveragepool : public Node {
 public:
  Globalaveragepool(Op type);

  ~Globalaveragepool();

  void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

  void set_attributes(std::map<std::string, std::vector<int>>& attrs);

  void init_kernel(std::shared_ptr<Kernel> kernel);

 private:
  GlobalaveragepoolParams* params_;
};

#endif  // GLOBALAVERAGEPOOL_H