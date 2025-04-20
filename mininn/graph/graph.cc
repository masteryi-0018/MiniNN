#include "mininn/graph/graph.h"

Graph::Graph() {}

void Graph::add_node(std::shared_ptr<Node> node, std::vector<int>& inputs, std::vector<int>& outputs, 
                     std::map<std::string, std::vector<int>>& attrs) {
    nodes_.emplace_back(node);
    std::vector<std::shared_ptr<Tensor>> tensors = get_tensors();

    std::vector<std::shared_ptr<Tensor>> input_tensors;
    for (uint i = 0; i < inputs.size(); ++i) {
        input_tensors.emplace_back(tensors[inputs[i]]);
    }
    node->set_input_tensors(input_tensors);

    std::vector<std::shared_ptr<Tensor>> output_tensors;
    for (uint i = 0; i < outputs.size(); ++i) {
        output_tensors.emplace_back(tensors[outputs[i]]);
    }
    node->set_output_tensors(output_tensors);

    node->set_attributes(attrs);
    return;
}

std::vector<std::shared_ptr<Node>> Graph::get_nodes() {
    return nodes_;
}

void Graph::add_tensor(std::vector<int>& shape, const void* data) {
    auto tensor = std::make_shared<Tensor>();
    tensor->set_shape(shape);
    tensor->set_rawdata(data);
    tensors_.emplace_back(tensor);
    return;
}

std::vector<std::shared_ptr<Tensor>> Graph::get_tensors() {
    return tensors_;
}

void Graph::set_inputs(std::vector<int>& inputs) {
    inputs_ = inputs;
}

std::vector<int>& Graph::get_inputs() {
    return inputs_;
}

void Graph::set_outputs(std::vector<int>& outputs) {
    outputs_ = outputs;
}

std::vector<int>& Graph::get_outputs() {
    return outputs_;
}