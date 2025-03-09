#ifndef GEMM_H
#define GEMM_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Gemm: public Node {
public:
    Gemm(Op type);

    ~Gemm();

    void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    GemmParams* params_;
};

#endif // GEMM_H