#ifndef ADD_COMPUTE_H
#define ADD_COMPUTE_H

#include "mininn/graph/kernel.h"

class AddCompute: public Kernel {
public:
    AddCompute();

    void run();

    void set_params(AddParams* params);

    AddParams* get_params();
private:
    AddParams* params_;
};

#endif // ADD_COMPUTE_H