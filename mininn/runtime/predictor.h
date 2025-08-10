#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "mininn/graph/graph.h"
#include "mininn/graph/kernel.h"

#include <vector>
#include <memory>
#include <future> // async

class Predictor {
public:
    Predictor(std::shared_ptr<Graph> graph);

    void prepare();

    void run();

    std::future<void> async_run_future();

    void async_run_callback(std::function<void()> callback);

    std::vector<std::shared_ptr<Tensor>> get_input_tensors();

    std::vector<std::shared_ptr<Tensor>> get_output_tensors();

    // debug
    std::vector<std::shared_ptr<Tensor>> dump_all_outputs();

private:
    std::shared_ptr<Graph> graph_;
    std::vector<std::shared_ptr<Kernel>> kernels_;
};

#endif // PREDICTOR_H