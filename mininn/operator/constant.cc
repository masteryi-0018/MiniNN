#include "mininn/operator/constant.h"
#include "mininn/graph/register.h"

Constant::Constant(Op type): Node(type) {
    params_ = new ConstantParams;
}

Constant::~Constant() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void Constant::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {}

void Constant::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Constant::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    params_->value = attrs["value"];
}

void Constant::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(CONSTANT, Constant);