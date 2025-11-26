#include "mininn/kernel/unsqueeze_compute.h"

#include <chrono>
#include <thread>

#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

UnsqueezeCompute::UnsqueezeCompute() {}

UnsqueezeCompute::~UnsqueezeCompute() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void unsqueeze_func(float* data_buffer, float* out_buffer, int size) {
  for (int i = 0; i < size; ++i) {
    out_buffer[i] = data_buffer[i];
  }
  // print_vector(input_shape); // []
  // print_vector(out_shape); // [1]
}

void UnsqueezeCompute::run() {
  LOG(INFO) << "kernel run start";
  auto start_time = std::chrono::high_resolution_clock::now();

  UnsqueezeParams* params = get_params();
  std::shared_ptr<Tensor> data = params->input1;
  std::shared_ptr<Tensor> out = params->output;

  int size = out->get_size();
  float* data_buffer = reinterpret_cast<float*>(data->get_buffer());
  float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

  unsqueeze_func(data_buffer, out_buffer, size);

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_time - start_time;
  LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
  LOG(INFO) << "kernel run end";
}

void UnsqueezeCompute::set_params(Params* params) {
  params_ = reinterpret_cast<UnsqueezeParams*>(params);
}

UnsqueezeParams* UnsqueezeCompute::get_params() { return params_; }

REGISTER_KERNEL(UNSQUEEZE, UnsqueezeCompute)