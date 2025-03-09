#ifndef RESHAPE_H
#define RESHAPE_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Reshape: public Node {
public:
    Reshape(Op type);

    ~Reshape();

    void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    ReshapeParams* params_;
};

#endif // RESHAPE_H