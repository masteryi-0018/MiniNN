#ifndef GATHER_COMPUTE_H
#define GATHER_COMPUTE_H

#include "mininn/graph/kernel.h"

class GatherCompute: public Kernel {
public:
    GatherCompute();

    ~GatherCompute();

    void run();

    void set_params(Params* params);

    GatherParams* get_params();

private:
    GatherParams* params_;
};

#endif // GATHER_COMPUTE_H