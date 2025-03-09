#include "mininn/operator/gemm.h"
#include "mininn/graph/register.h"

Gemm::Gemm(Op type): Node(type) {
    params_ = new GemmParams;
}

Gemm::~Gemm() {
    delete params_;
}

void Gemm::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
    params_->input2 = tensors[1];
    params_->input3 = tensors[2];
}

void Gemm::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Gemm::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(GEMM, Gemm);