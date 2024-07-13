#include "mininn/operator/add.h"
#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

Add::Add(Op type): Node(type) {
    params_ = new AddParams;
}

void Add::set_input_tensors(std::vector<std::shared_ptr<Tensor>> tensors) {
    params_->input1 = tensors[0];
    params_->input2 = tensors[1];
}

void Add::set_output_tensors(std::vector<std::shared_ptr<Tensor>> tensors) {
    params_->output = tensors[0];
}

void Add::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(ADD, Add);