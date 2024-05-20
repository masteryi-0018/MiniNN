#include "node.h"

Node::Node() {}

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