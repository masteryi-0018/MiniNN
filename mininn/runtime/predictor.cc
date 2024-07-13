#include "mininn/runtime/predictor.h"

Predictor::Predictor(std::shared_ptr<Graph> graph): graph_(graph) {}

void Predictor::prepare() {
    auto nodes_ = graph_->get_nodes();
    for (auto node: nodes_) {
        std::shared_ptr<Kernel> kernel = node->create_kernel();
        kernels_.emplace_back(kernel);
    }
}

std::vector<std::shared_ptr<Kernel>> Predictor::get_kernels() {
    return kernels_;
}

void Predictor::run() {
    for (auto kernel: kernels_) {
        kernel->run();
    }
}

std::vector<std::shared_ptr<Tensor>> Predictor::get_input_tensors() {
    std::vector<int> inputs = graph_->get_inputs();
    std::vector<std::shared_ptr<Tensor>> tensors;
    for (int i = 0; i < inputs.size(); ++i) {
        std::shared_ptr<Tensor> tensor = graph_->get_tensors()[inputs[i]];
        tensors.emplace_back(tensor);
    }
    return tensors;
}

std::vector<std::shared_ptr<Tensor>> Predictor::get_output_tensors() {
    std::vector<int> outputs = graph_->get_outputs();
    std::vector<std::shared_ptr<Tensor>> tensors;
    for (int i = 0; i < outputs.size(); ++i) {
        std::shared_ptr<Tensor> tensor = graph_->get_tensors()[outputs[i]];
        tensors.emplace_back(tensor);
    }
    return tensors;
}