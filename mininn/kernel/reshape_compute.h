#ifndef RESHAPE_COMPUTE_H
#define RESHAPE_COMPUTE_H

#include "mininn/graph/kernel.h"

class ReshapeCompute: public Kernel {
public:
    ReshapeCompute();

    ~ReshapeCompute();

    void run();

    void set_params(Params* params);

    ReshapeParams* get_params();

private:
    ReshapeParams* params_;
};

#endif // RESHAPE_COMPUTE_H