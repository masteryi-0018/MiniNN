#include "mininn/operator/conv.h"
#include "mininn/graph/register.h"

Conv::Conv(Op type): Node(type) {
    params_ = new ConvParams;
}

Conv::~Conv() {
    delete params_;
}

void Conv::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
    params_->input2 = tensors[1];
    params_->input3 = tensors[2];
}

void Conv::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Conv::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(CONV, Conv);