#include "mininn/kernel/unsqueeze_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

UnsqueezeCompute::UnsqueezeCompute() {}

UnsqueezeCompute::~UnsqueezeCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void unsqueeze_func(float* input_buffer, float* out_buffer,
                    std::vector<int> input_shape, std::vector<int> out_shape) {
    //
}

void UnsqueezeCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    UnsqueezeParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    unsqueeze_func(input_buffer,out_buffer, input_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void UnsqueezeCompute::set_params(Params* params) {
    params_ = reinterpret_cast<UnsqueezeParams*>(params);
}

UnsqueezeParams* UnsqueezeCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(UNSQUEEZE, UnsqueezeCompute);