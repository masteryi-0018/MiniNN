#ifndef ADD_COMPUTE_H
#define ADD_COMPUTE_H

#include "mininn/graph/kernel.h"

class AddCompute: public Kernel {
public:
    AddCompute();

    void run();

    // inherit from class Kernel, input params should be totally the same
    // and use reinterpret_cast t AddParams
    void set_params(Params* params);

    AddParams* get_params();
private:
    AddParams* params_;
};

#endif // ADD_COMPUTE_H