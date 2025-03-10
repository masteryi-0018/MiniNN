#include "mininn/kernel/conv_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo

ConvCompute::ConvCompute() {}

ConvCompute::~ConvCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void conv_func(float* x_buffer, float* y_buffer, float* out_buffer, int start, int end) {
    //
}

void ConvCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ConvParams* params = get_params();
    std::shared_ptr<Tensor> x = params->input1;
    std::shared_ptr<Tensor> y = params->input2;
    std::shared_ptr<Tensor> out = params->output;

    int size = x->get_size();
    float* x_buffer = reinterpret_cast<float*>(x->get_buffer());
    float* y_buffer = reinterpret_cast<float*>(y->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    conv_func(x_buffer, y_buffer, out_buffer, 0, size);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void ConvCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ConvParams*>(params);
}

ConvParams* ConvCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(CONV, ConvCompute);