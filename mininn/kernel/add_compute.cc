#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

AddCompute::AddCompute() {}

void AddCompute::run() {
    LOG("kernel run start");
    AddParams* params = get_params();
    std::shared_ptr<Tensor> x = params->input1;
    std::shared_ptr<Tensor> y = params->input2;
    std::shared_ptr<Tensor> out = params->output;

    int size = x->get_size();

    float* x_buffer = reinterpret_cast<float*>(x->get_buffer());
    float* y_buffer = reinterpret_cast<float*>(y->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    for (int i = 0; i < size; ++i) {
        out_buffer[i] = x_buffer[i] + y_buffer[i];
    }
    LOG("kernel run end");
}

void AddCompute::set_params(Params* params) {
    params_ = reinterpret_cast<AddParams*>(params);
}

AddParams* AddCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(ADD, AddCompute);