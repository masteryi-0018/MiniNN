#include "mininn/operator/globalaveragepool.h"
#include "mininn/graph/register.h"

Globalaveragepool::Globalaveragepool(Op type): Node(type) {
    params_ = new GlobalaveragepoolParams;
}

Globalaveragepool::~Globalaveragepool() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void Globalaveragepool::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
}

void Globalaveragepool::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Globalaveragepool::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    return;
}

void Globalaveragepool::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(GLOBALAVERAGEPOOL, Globalaveragepool);