#include "mininn/operator/conv.h"

#include "mininn/graph/register.h"

Conv::Conv(Op type) : Node(type) { params_ = new ConvParams; }

Conv::~Conv() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void Conv::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
  params_->input1 = tensors[0];
  params_->input2 = tensors[1];
  params_->input3 = tensors[2];
}

void Conv::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
  params_->output = tensors[0];
}

void Conv::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
  params_->dilations = attrs["dilations"];
  params_->group = attrs["group"];
  params_->kernel_shape = attrs["kernel_shape"];
  params_->pads = attrs["pads"];
  params_->strides = attrs["strides"];
}

void Conv::init_kernel(std::shared_ptr<Kernel> kernel) {
  kernel->set_params(params_);
}

REGISTER_OP(CONV, Conv)