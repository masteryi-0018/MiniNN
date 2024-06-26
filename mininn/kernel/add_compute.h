#ifndef ADD_COMPUTE_H
#define ADD_COMPUTE_H

#include "kernel.h"

class AddCompute: public Kernel {
public:
    AddCompute();

    void run();
};

#endif // ADD_COMPUTE_H