#include "mininn/kernel/globalaveragepool_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

GlobalaveragepoolCompute::GlobalaveragepoolCompute() {}

GlobalaveragepoolCompute::~GlobalaveragepoolCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void globalaveragepool_func(float* input_buffer, float* out_buffer, std::vector<int> out_shape) {
    // 
}

void GlobalaveragepoolCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    GlobalaveragepoolParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    globalaveragepool_func(input_buffer, out_buffer, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";

}

void GlobalaveragepoolCompute::set_params(Params* params) {
    params_ = reinterpret_cast<GlobalaveragepoolParams*>(params);
}

GlobalaveragepoolParams* GlobalaveragepoolCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(GLOBALAVERAGEPOOL, GlobalaveragepoolCompute);