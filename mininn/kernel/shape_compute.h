#ifndef SHAPE_COMPUTE_H
#define SHAPE_COMPUTE_H

#include "mininn/graph/kernel.h"

class ShapeCompute: public Kernel {
public:
    ShapeCompute();

    ~ShapeCompute();

    void run();

    void set_params(Params* params);

    ShapeParams* get_params();

private:
    ShapeParams* params_;
};

#endif // SHAPE_COMPUTE_H