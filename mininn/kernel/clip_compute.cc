#include "mininn/kernel/clip_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

ClipCompute::ClipCompute() {}

ClipCompute::~ClipCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void clip_func(float* input_buffer, float* out_buffer, int size) {
    // 
}

void ClipCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ClipParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> out = params->output;

    int size = input->get_size();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    clip_func(input_buffer, out_buffer, size);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void ClipCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ClipParams*>(params);
}

ClipParams* ClipCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(CLIP, ClipCompute);