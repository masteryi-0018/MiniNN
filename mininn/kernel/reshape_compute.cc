#include "mininn/kernel/reshape_compute.h"

#include <chrono>
#include <thread>

#include "mininn/graph/register.h"
#include "mininn/utils/log.h"

ReshapeCompute::ReshapeCompute() {}

ReshapeCompute::~ReshapeCompute() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

void reshape_func(float* input_buffer, float* shape_buffer,
                  std::shared_ptr<Tensor>& out, std::vector<int> shape_val,
                  int size) {
  // output of reshape not malloc the real buffer
  int sum = 1;
  std::vector<int> out_shape;
  for (int i = 0; i < shape_val[0]; ++i) {
    // only 1 dim can be -1
    if (shape_buffer[i] == -1) {
      int new_size = size / sum;
      out_shape.emplace_back(new_size);
    } else {
      out_shape.emplace_back(static_cast<int>(shape_buffer[i]));
      sum *= static_cast<int>(shape_buffer[i]);
    }
  }
  out->set_shape(out_shape);

  float* out_buffer = reinterpret_cast<float*>(out->get_buffer());
  for (int i = 0; i < size; ++i) {
    out_buffer[i] = input_buffer[i];
  }
}

void ReshapeCompute::run() {
  LOG(INFO) << "kernel run start";
  auto start_time = std::chrono::high_resolution_clock::now();

  ReshapeParams* params = get_params();
  std::shared_ptr<Tensor> data = params->input1;
  std::shared_ptr<Tensor> shape = params->input2;
  std::shared_ptr<Tensor> out = params->output;

  int size = data->get_size();
  std::vector<int> shape_val = shape->get_shape();

  float* data_buffer = reinterpret_cast<float*>(data->get_buffer());
  float* shape_buffer = reinterpret_cast<float*>(shape->get_buffer());

  reshape_func(data_buffer, shape_buffer, out, shape_val, size);

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_time - start_time;
  LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
  LOG(INFO) << "kernel run end";
}

void ReshapeCompute::set_params(Params* params) {
  params_ = reinterpret_cast<ReshapeParams*>(params);
}

ReshapeParams* ReshapeCompute::get_params() { return params_; }

REGISTER_KERNEL(RESHAPE, ReshapeCompute);