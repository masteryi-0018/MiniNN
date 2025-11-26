#include "mininn/kernel/concat_compute.h"

#include <chrono>
#include <thread>

#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

ConcatCompute::ConcatCompute() {}

ConcatCompute::~ConcatCompute() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void concat_func(std::vector<std::shared_ptr<Tensor>> inputs,
                 float* out_buffer) {
  // todo: only support axis == 0
  // int axis_val = axis[0];
  int input_size = inputs[0]->get_size();
  int offset = 0;
  for (uint32_t i = 0; i < inputs.size(); ++i) {
    float* temp_buffer = reinterpret_cast<float*>(inputs[i]->get_buffer());
    for (int k = 0; k < input_size; ++k) {
      out_buffer[offset + k] = temp_buffer[k];
    }
    offset += input_size;
  }
}

void ConcatCompute::run() {
  LOG(INFO) << "kernel run start";
  auto start_time = std::chrono::high_resolution_clock::now();

  ConcatParams* params = get_params();
  std::vector<std::shared_ptr<Tensor>> inputs = params->inputs;
  std::shared_ptr<Tensor> out = params->output;

  float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

  concat_func(inputs, out_buffer);

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_time - start_time;
  LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
  LOG(INFO) << "kernel run end";
}

void ConcatCompute::set_params(Params* params) {
  params_ = reinterpret_cast<ConcatParams*>(params);
}

ConcatParams* ConcatCompute::get_params() { return params_; }

REGISTER_KERNEL(CONCAT, ConcatCompute)