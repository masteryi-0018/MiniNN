#include "mininn/operator/add.h"
#include "mininn/graph/register.h"

Add::Add(Op type): Node(type) {
    params_ = new AddParams;
}

Add::~Add() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void Add::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
    params_->input2 = tensors[1];
}

void Add::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Add::set_attributes(std::map<std::string, std::vector<int>>& attrs) {

}

void Add::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(ADD, Add);