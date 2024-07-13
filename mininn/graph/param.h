#ifndef PARAM_H
#define PARAM_H

#include "mininn/graph/tensor.h"

#include <memory>

struct Params {};

struct AddParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> input2;
    std::shared_ptr<Tensor> output;
};

#endif // PARAM_H