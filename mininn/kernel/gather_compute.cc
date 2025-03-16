#include "mininn/kernel/gather_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

GatherCompute::GatherCompute() {}

GatherCompute::~GatherCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void gather_func(float* input1_buffer, float* input2_buffer, float* out_buffer,
                 std::vector<int> input_shape, std::vector<int> out_shape) {
    // 
}

void GatherCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    GatherParams* params = get_params();
    std::shared_ptr<Tensor> input1 = params->input1;
    std::shared_ptr<Tensor> input2 = params->input2;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> input_shape = input1->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input1_buffer = reinterpret_cast<float*>(input1->get_buffer());
    float* input2_buffer = reinterpret_cast<float*>(input2->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    gather_func(input1_buffer, input2_buffer, out_buffer, input_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void GatherCompute::set_params(Params* params) {
    params_ = reinterpret_cast<GatherParams*>(params);
}

GatherParams* GatherCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(GATHER, GatherCompute);