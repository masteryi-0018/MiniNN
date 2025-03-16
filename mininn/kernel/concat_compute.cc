#include "mininn/kernel/concat_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

ConcatCompute::ConcatCompute() {}

ConcatCompute::~ConcatCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void concat_func(std::vector<std::shared_ptr<Tensor>> inputs, float* out_buffer, int size) {
    // 
}

void ConcatCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ConcatParams* params = get_params();
    std::vector<std::shared_ptr<Tensor>> inputs = params->inputs;
    std::shared_ptr<Tensor> out = params->output;

    int size = out->get_size();
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    concat_func(inputs, out_buffer, size);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void ConcatCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ConcatParams*>(params);
}

ConcatParams* ConcatCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(CONCAT, ConcatCompute);