#ifndef GLOBALAVERAGEPOOL_COMPUTE_H
#define GLOBALAVERAGEPOOL_COMPUTE_H

#include "mininn/graph/kernel.h"

class GlobalaveragepoolCompute: public Kernel {
public:
    GlobalaveragepoolCompute();

    ~GlobalaveragepoolCompute();

    void run();

    void set_params(Params* params);

    GlobalaveragepoolParams* get_params();

private:
    GlobalaveragepoolParams* params_;
};

#endif // GLOBALAVERAGEPOOL_COMPUTE_H