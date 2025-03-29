#include "mininn/operator/gather.h"
#include "mininn/graph/register.h"

Gather::Gather(Op type): Node(type) {
    params_ = new GatherParams;
}

Gather::~Gather() {
    delete params_;
}

void Gather::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
    params_->input2 = tensors[1];
}

void Gather::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Gather::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    params_->axis = attrs["axis"];
}

void Gather::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(GATHER, Gather);