#ifndef KERNEL_H
#define KERNEL_H

#include "mininn/graph/param.h"

class Kernel {
public:
    Kernel();

    virtual void run() = 0;

    virtual void set_params(Params* params) = 0;

    virtual Params* get_params() = 0;
};

#endif // KERNEL_H