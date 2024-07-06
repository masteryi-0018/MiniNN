#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

AddCompute::AddCompute() {}

void AddCompute::run() {
    LOG("kernel run start");
    AddParams* params = get_params();
    Tensor* x = params->input1;
    Tensor* y = params->input2;
    Tensor* out = params->output;
    LOG("kernel run end");
}

void AddCompute::set_params(AddParams* params) {
    LOG("set_params"); // todo: can't be printed
    params_ = params;
}

AddParams* AddCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(ADD, AddCompute);