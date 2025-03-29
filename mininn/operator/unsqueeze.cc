#include "mininn/operator/unsqueeze.h"
#include "mininn/graph/register.h"

Unsqueeze::Unsqueeze(Op type): Node(type) {
    params_ = new UnsqueezeParams;
}

Unsqueeze::~Unsqueeze() {
    delete params_;
}

void Unsqueeze::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
}

void Unsqueeze::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Unsqueeze::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    params_->axes = attrs["axes"];
}

void Unsqueeze::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(UNSQUEEZE, Unsqueeze);