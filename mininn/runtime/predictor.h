#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "mininn/graph/graph.h"
#include "mininn/graph/kernel.h"

#include <vector>
#include <memory>

class Predictor {
public:
    Predictor(std::shared_ptr<Graph> graph);

    void prepare();

    void run();

    std::vector<std::shared_ptr<Tensor>> get_input_tensors();

    std::vector<std::shared_ptr<Tensor>> get_output_tensors();

private:
    std::shared_ptr<Graph> graph_;
    std::vector<std::shared_ptr<Kernel>> kernels_;
};

#endif // PREDICTOR_H