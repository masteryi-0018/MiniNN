#include "mininn/operator/add.h"
#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

Add::Add(Op type): Node(type) {
    params_ = new AddParams;
    // todo: params_ should point to graph's tensor
    params_->input1 = new Tensor();
    params_->input2 = new Tensor();
    params_->output = new Tensor();
}

void Add::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(ADD, Add);