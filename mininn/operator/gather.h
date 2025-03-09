#ifndef GATHER_H
#define GATHER_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Gather: public Node {
public:
    Gather(Op type);

    ~Gather();

    void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    GatherParams* params_;
};

#endif // GATHER_H