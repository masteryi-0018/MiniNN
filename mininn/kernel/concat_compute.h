#ifndef CONCAT_COMPUTE_H
#define CONCAT_COMPUTE_H

#include "mininn/graph/kernel.h"

class ConcatCompute: public Kernel {
public:
    ConcatCompute();

    ~ConcatCompute();

    void run();

    void set_params(Params* params);

    ConcatParams* get_params();

private:
    ConcatParams* params_;
};

#endif // CONCAT_COMPUTE_H