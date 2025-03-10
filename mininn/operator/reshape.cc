#include "mininn/operator/reshape.h"
#include "mininn/graph/register.h"

Reshape::Reshape(Op type): Node(type) {
    params_ = new ReshapeParams;
}

Reshape::~Reshape() {
    delete params_;
}

void Reshape::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
}

void Reshape::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Reshape::set_attributes(std::map<std::string, std::vector<int>>& attrs) {

}

void Reshape::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(RESHAPE, Reshape);