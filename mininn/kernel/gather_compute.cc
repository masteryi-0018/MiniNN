#include "mininn/kernel/gather_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo
#include "mininn/utils/utils.h"

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

void gather_func(float* data_buffer, float* indices_buffer, float* out_buffer, std::vector<int> axis,
                 std::vector<int> indices_shape, std::vector<int> out_shape) {
    // todo: only support axis == 0
    // todo: only support indices_shape is 1 dim
    // todo: only support data_shape is 1 dim
    // int axis_val = axis[0];
    // int offset = 0;
    for (int i = 0; i < indices_shape[0]; ++i) {
        int index = (int)(indices_buffer[i]);
        for (int j = 0; j < out_shape[0]; ++j) {
            out_buffer[j] = data_buffer[index];
        }
    }
    // print_vector(indices_shape);
    // print_vector(out_shape); // don't need to pass T when T is in params
}

void GatherCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    GatherParams* params = get_params();
    std::shared_ptr<Tensor> data = params->input1;
    std::shared_ptr<Tensor> indices = params->input2;
    std::shared_ptr<Tensor> out = params->output;
    std::vector<int> axis = params_->axis;

    // std::vector<int> input_shape = data->get_shape();
    std::vector<int> indices_shape = indices->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* data_buffer = reinterpret_cast<float*>(data->get_buffer());
    float* indices_buffer = reinterpret_cast<float*>(indices->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    gather_func(data_buffer, indices_buffer, out_buffer, axis, indices_shape, out_shape);

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