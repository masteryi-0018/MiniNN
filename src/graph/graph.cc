#include "graph.h"

Graph::Graph(std::vector<Node*>& nodes, std::vector<Tensor*>& tensors) {
    nodes_ = nodes;
    tensors_ = tensors;
}

std::vector<int> Graph::get_inputs() const {
    return inputs_;
}

void Graph::set_inputs(std::vector<int>& indices) {
    inputs_ = indices;
}

std::vector<int> Graph::get_outputs() const {
    return outputs_;
}

void Graph::set_outputs(std::vector<int>& indices) {
    outputs_ = indices;
}