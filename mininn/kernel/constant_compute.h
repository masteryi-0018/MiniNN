#ifndef CONSTANT_COMPUTE_H
#define CONSTANT_COMPUTE_H

#include "mininn/graph/kernel.h"

class ConstantCompute: public Kernel {
public:
    ConstantCompute();

    ~ConstantCompute();

    void run();

    void set_params(Params* params);

    ConstantParams* get_params();

private:
    ConstantParams* params_;
};

#endif // CONSTANT_COMPUTE_H