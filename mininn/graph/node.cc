#include "mininn/graph/node.h"
#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

Node::Node(Op type): op_type_(type) {}

void Node::set_inputs(std::vector<int>& indices) {
    inputs_ = indices;
}

std::vector<int> Node::get_inputs() const {
    return inputs_;
}

void Node::set_outputs(std::vector<int>& indices) {
    outputs_ = indices;
}

std::vector<int> Node::get_outputs() const {
    return outputs_;
}

std::shared_ptr<Kernel> Node::create_kernel() {
    std::shared_ptr<Kernel> kernel = KernelFactory::Global().Create(op_type_);
    init_kernel(kernel);
    return kernel;
}

void Node::init_kernel(std::shared_ptr<Kernel> kernel) {
    LOG("shouldn't be printed\n");
}

// void Node::set_input_tensors(std::vector<std::shared_ptr<Tensor>> tensors) {}

// void Node::set_output_tensors(std::vector<std::shared_ptr<Tensor>> tensors) {}