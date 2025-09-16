#include "mininn/operator/shape.h"
#include "mininn/graph/register.h"

Shape::Shape(Op type): Node(type) {
    params_ = new ShapeParams;
}

Shape::~Shape() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void Shape::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
}

void Shape::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Shape::set_attributes(std::map<std::string, std::vector<int>>& attrs) {
    if (attrs.size() != 0) {
        LOG(INFO) << "Shape should not have attributes.";
    }
    return;
}

void Shape::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(SHAPE, Shape);