#include "mininn/kernel/constant_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h"

#include <thread>
#include <chrono>

ConstantCompute::ConstantCompute() {}

ConstantCompute::~ConstantCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void constant_func(float* out_buffer, std::vector<int> value, int size) {
    int val = value[0];
    for (int i = 0; i < size; ++i) {
        out_buffer[i] = val;
    }
}

void ConstantCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ConstantParams* params = get_params();
    std::shared_ptr<Tensor> out = params->output;
    std::vector<int> value = params_->value;

    int size = out->get_size();
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    constant_func(out_buffer, value, size);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";
}

void ConstantCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ConstantParams*>(params);
}

ConstantParams* ConstantCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(CONSTANT, ConstantCompute);