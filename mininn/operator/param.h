#ifndef PARAM_H
#define PARAM_H

#include "tensor.h"

struct Params {};

struct AddParams : Params {
    Tensor* input1;
    Tensor* input2;
    Tensor* output;
};

#endif // PARAM_H