#ifndef PARAM_H
#define PARAM_H

#include "mininn/graph/tensor.h"

struct Params {};

struct AddParams: Params {
    Tensor* input1;
    Tensor* input2;
    Tensor* output;
};

#endif // PARAM_H