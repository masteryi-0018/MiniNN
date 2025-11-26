#include "mininn/kernel/globalaveragepool_compute.h"

#include <chrono>
#include <thread>

#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

GlobalaveragepoolCompute::GlobalaveragepoolCompute() {}

GlobalaveragepoolCompute::~GlobalaveragepoolCompute() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void globalaveragepool_func(float* input_buffer, float* out_buffer,
                            std::vector<int> input_shape) {
  // reserve output dims. such as [1, 1280, 1, 1]
  int batch_size = input_shape[0];
  int channels = input_shape[1];
  int height = input_shape[2];
  int width = input_shape[3];

  int spatial_size = height * width;

  for (int n = 0; n < batch_size; ++n) {
    for (int c = 0; c < channels; ++c) {
      float sum = 0.0f;

      for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          int input_idx = n * channels * height * width + c * height * width +
                          h * width + w;
          sum += input_buffer[input_idx];
        }
      }

      int output_idx = n * channels + c;
      out_buffer[output_idx] = sum / spatial_size;
    }
  }
}

void GlobalaveragepoolCompute::run() {
  LOG(INFO) << "kernel run start";
  auto start_time = std::chrono::high_resolution_clock::now();

  GlobalaveragepoolParams* params = get_params();
  std::shared_ptr<Tensor> input = params->input1;
  std::shared_ptr<Tensor> out = params->output;

  std::vector<int> input_shape = input->get_shape();
  float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
  float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

  globalaveragepool_func(input_buffer, out_buffer, input_shape);

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

REGISTER_KERNEL(GLOBALAVERAGEPOOL, GlobalaveragepoolCompute)