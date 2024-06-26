#include "node.h"
#include "register.h"

Node::Node() {}

Node::Node(Op type): op_type_(type) {}

std::vector<int> Node::get_inputs() const {
    return inputs_;
}

void Node::set_inputs(std::vector<int>& indices) {
    inputs_ = indices;
}

std::vector<int> Node::get_outputs() const {
    return outputs_;
}

void Node::set_outputs(std::vector<int>& indices) {
    outputs_ = indices;
}

std::shared_ptr<Kernel> Node::create_kernel() {
    std::shared_ptr<Kernel> kernel = KernelFactory::Global().Create(op_type_);
    return kernel;
}