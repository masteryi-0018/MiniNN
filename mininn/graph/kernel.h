#ifndef KERNEL_H
#define KERNEL_H

#include "mininn/graph/param.h"

class Kernel {
public:
    Kernel();

    // must be a virtual func
    virtual void run();

    // must be a virtual func
    virtual void set_params(Params* params);

    Params* get_params();
};

#endif // KERNEL_H