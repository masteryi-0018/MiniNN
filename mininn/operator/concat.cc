#include "mininn/operator/concat.h"
#include "mininn/graph/register.h"

Concat::Concat(Op type): Node(type) {
    params_ = new ConcatParams;
}

Concat::~Concat() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void Concat::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    for (uint32_t i = 0; i < tensors.size(); ++i) {
        params_->inputs.emplace_back(tensors[i]);
    }
}

void Concat::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Concat::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    params_->axis = attrs["axis"];
}

void Concat::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(CONCAT, Concat);