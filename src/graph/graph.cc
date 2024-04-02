#include "graph.h"

Graph::Graph() {}

void Graph::add_node(std::shared_ptr<Node> node, std::vector<int> inputs, std::vector<int> outputs) {
    nodes_.emplace_back(node);
    node->set_inputs(inputs);
    node->set_outputs(outputs);
    return;
}

std::vector<std::shared_ptr<Node>> Graph::get_nodes() {
    return nodes_;
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