#include "mininn/operator/clip.h"

#include "mininn/graph/register.h"

Clip::Clip(Op type) : Node(type) { params_ = new ClipParams; }

Clip::~Clip() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void Clip::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
  params_->input1 = tensors[0];
}

void Clip::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
  params_->output = tensors[0];
}

void Clip::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
  params_->max = attrs["max"];
  params_->min = attrs["min"];
}

void Clip::init_kernel(std::shared_ptr<Kernel> kernel) {
  kernel->set_params(params_);
}

REGISTER_OP(CLIP, Clip)