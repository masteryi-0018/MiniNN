#include "mininn/kernel/reshape_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

ReshapeCompute::ReshapeCompute() {}

ReshapeCompute::~ReshapeCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void reshape_func(float* input_buffer, float* out_buffer,
                  std::vector<int> input_shape, std::vector<int> out_shape) {
    // 
}

void ReshapeCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ReshapeParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    reshape_func(input_buffer, out_buffer, input_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void ReshapeCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ReshapeParams*>(params);
}

ReshapeParams* ReshapeCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(RESHAPE, ReshapeCompute);