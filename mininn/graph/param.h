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

struct ConvParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> input2;
    std::shared_ptr<Tensor> input3;
    std::shared_ptr<Tensor> output;
    std::vector<int> dilations;
    std::vector<int> group;
    std::vector<int> kernel_shape;
    std::vector<int> pads;
    std::vector<int> strides;
};

struct ClipParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> output;
    std::vector<int> max;
    std::vector<int> min;
};

// todo: ai.onnx v1
struct ShapeParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> output;
};

struct GatherParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> input2;
    std::shared_ptr<Tensor> output;
};

struct UnsqueezeParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> output;
};

struct ConcatParams: Params {
    std::vector<std::shared_ptr<Tensor>> inputs;
    std::shared_ptr<Tensor> output;
};

struct GlobalaveragepoolParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> output;
};

struct ReshapeParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> output;
};

struct GemmParams: Params {
    std::shared_ptr<Tensor> input1;
    std::shared_ptr<Tensor> input2;
    std::shared_ptr<Tensor> input3;
    std::shared_ptr<Tensor> output;
};

struct ConstantParams: Params {
    std::shared_ptr<Tensor> output;
    std::vector<int> value;
};

#endif // PARAM_H