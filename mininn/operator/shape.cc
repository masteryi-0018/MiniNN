#include "mininn/operator/shape.h"
#include "mininn/graph/register.h"

Shape::Shape(Op type): Node(type) {
    params_ = new ShapeParams;
}

Shape::~Shape() {
    delete params_;
}

void Shape::set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->input1 = tensors[0];
}

void Shape::set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors) {
    params_->output = tensors[0];
}

void Shape::init_kernel(std::shared_ptr<Kernel> kernel) {
    kernel->set_params(params_);
}

REGISTER_OP(SHAPE, Shape);