#include "mininn/kernel/gemm_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

GemmCompute::GemmCompute() {}

GemmCompute::~GemmCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void gemm_func(float* input_buffer, float* weight_buffer, float* bias_buffer, float* out_buffer,
               std::vector<int> input_shape, std::vector<int> weight_shape,
               std::vector<int> bias_shape, std::vector<int> out_shape) {
    // 
}

void GemmCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    GemmParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> weight = params->input2;
    std::shared_ptr<Tensor> bias = params->input3;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> weight_shape = weight->get_shape();
    std::vector<int> bias_shape = bias->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* weight_buffer = reinterpret_cast<float*>(weight->get_buffer());
    float* bias_buffer = reinterpret_cast<float*>(bias->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    gemm_func(input_buffer, weight_buffer, bias_buffer, out_buffer,
              input_shape, weight_shape, bias_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void GemmCompute::set_params(Params* params) {
    params_ = reinterpret_cast<GemmParams*>(params);
}

GemmParams* GemmCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(GEMM, GemmCompute);