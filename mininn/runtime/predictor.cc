#include "mininn/runtime/predictor.h"

Predictor::Predictor(std::shared_ptr<Graph> graph): graph_(graph) {}

void Predictor::prepare() {
    auto nodes_ = graph_->get_nodes();
    for (auto node: nodes_) {
        std::shared_ptr<Kernel> kernel = node->create_kernel();
        kernels_.emplace_back(kernel);
    }
}

void Predictor::run() {
    for (auto kernel: kernels_) {
        kernel->run();
    }
}

std::future<void> Predictor::async_run_future() {
    std::future<void> future = std::async(std::launch::async,
                                          &Predictor::run,
                                          this);
    return future;
}

void Predictor::async_run_callback(std::function<void()> callback) {
    auto thread_func = [this, callback]() {
        this->run();
        callback();
    };

    std::thread t(thread_func);
    t.detach();
    return;
}

std::vector<std::shared_ptr<Tensor>> Predictor::get_input_tensors() {
    std::vector<std::shared_ptr<Tensor>> tensors = graph_->get_tensors();
    std::vector<int> inputs = graph_->get_inputs();
    std::vector<std::shared_ptr<Tensor>> input_tensors;
    for (int i = 0; i < inputs.size(); ++i) {
        input_tensors.emplace_back(tensors[inputs[i]]);
    }
    return input_tensors;
}

std::vector<std::shared_ptr<Tensor>> Predictor::get_output_tensors() {
    std::vector<std::shared_ptr<Tensor>> tensors = graph_->get_tensors();
    std::vector<int> outputs = graph_->get_outputs();
    std::vector<std::shared_ptr<Tensor>> output_tensors;
    for (int i = 0; i < outputs.size(); ++i) {
        output_tensors.emplace_back(tensors[outputs[i]]);
    }
    return output_tensors;
}