#ifndef GEMM_COMPUTE_H
#define GEMM_COMPUTE_H

#include "mininn/graph/kernel.h"

class GemmCompute: public Kernel {
public:
    GemmCompute();

    ~GemmCompute();

    void run();

    void set_params(Params* params);

    GemmParams* get_params();

private:
    GemmParams* params_;
};

#endif // GEMM_COMPUTE_H