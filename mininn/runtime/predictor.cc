#include "mininn/runtime/predictor.h"

Predictor::Predictor(std::shared_ptr<Graph> graph): graph_(graph) {}

void Predictor::prepare() {
    auto nodes_ = graph_->get_nodes();
    for (auto node: nodes_) {
        std::shared_ptr<Kernel> kernel = node->create_kernel();
        kernels_.emplace_back(kernel);
    }
}

std::vector<std::shared_ptr<Kernel>> Predictor::get_kernels() const {
    return kernels_;
}

void Predictor::run() {
    for (auto kernel: kernels_) {
        kernel->run();
    }
}