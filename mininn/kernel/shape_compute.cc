#include "mininn/kernel/shape_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h"

#include <thread>
#include <chrono>

ShapeCompute::ShapeCompute() {}

ShapeCompute::~ShapeCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void shape_func(float* out_buffer, std::vector<int> input_shape, std::vector<int> out_shape) {
    for (int i = 0; i < out_shape[0]; ++i) {
        out_buffer[i] = input_shape[i];
    }
}

void ShapeCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ShapeParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    shape_func(out_buffer, input_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";
}

void ShapeCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ShapeParams*>(params);
}

ShapeParams* ShapeCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(SHAPE, ShapeCompute);