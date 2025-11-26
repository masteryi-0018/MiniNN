#include "mininn/kernel/gather_compute.h"

#include <chrono>
#include <numeric>
#include <thread>

#include "mininn/graph/register.h"
#include "mininn/utils/log.h"
#include "mininn/utils/utils.h"

GatherCompute::GatherCompute() {}

GatherCompute::~GatherCompute() {
  if (params_ != nullptr) {
    params_ = nullptr;
    delete params_;
  }
}

std::vector<int> compute_strides(const std::vector<int>& shape) {
  std::vector<int> strides(shape.size(), 1);
    for (int i = static_cast<int>(shape.size()) - 2; i >= 0; --i) {
    strides[i] = strides[i + 1] * shape[i + 1];
  }
  return strides;
}

int get_flat_index(const std::vector<int>& idx,
                   const std::vector<int>& strides) {
  int flat_idx = 0;
  for (size_t i = 0; i < idx.size(); ++i) {
    flat_idx += idx[i] * strides[i];
  }
  return flat_idx;
}

void gather_func(float* data_buffer, float* indices_buffer, float* out_buffer,
                 std::vector<int> axis, std::vector<int> indices_shape,
                 std::vector<int> data_shape, std::vector<int> out_shape) {
  // todo: only support axis == 0
  // todo: only support indices_shape is 1 dim
  int gather_axis = axis[0];
  if (indices_shape.data() == nullptr) {
    indices_shape = std::vector<int>(1, 1);  // treat empty indices as scalar
  }
  out_shape.clear();
  out_shape.insert(out_shape.end(), data_shape.begin(),
                   data_shape.begin() + gather_axis);
  out_shape.insert(out_shape.end(), indices_shape.begin(), indices_shape.end());
  out_shape.insert(out_shape.end(), data_shape.begin() + gather_axis + 1,
                   data_shape.end());
  int ndim = static_cast<int>(out_shape.size());

  std::vector<int> data_strides = compute_strides(data_shape);
  std::vector<int> indices_strides = compute_strides(indices_shape);
  std::vector<int> out_strides = compute_strides(out_shape);

  int out_size = std::accumulate(out_shape.begin(), out_shape.end(), 1,
                                 std::multiplies<int>());

  for (int i = 0; i < out_size; ++i) {
    std::vector<int> out_idx(ndim);
    int tmp = i;
    for (int d = 0; d < ndim; ++d) {
      out_idx[d] = tmp / out_strides[d];
      tmp = tmp % out_strides[d];
    }

    std::vector<int> indices_idx(indices_shape.size());
    for (size_t d = 0; d < indices_shape.size(); ++d) {
      indices_idx[d] = out_idx[d];
    }

    int idx_in_axis = static_cast<int>(
        indices_buffer[get_flat_index(indices_idx, indices_strides)]);
    out_idx[gather_axis] = idx_in_axis;
    // debug
    // print_vector(out_idx);

    int data_flat_idx = get_flat_index(out_idx, data_strides);
    out_buffer[i] = data_buffer[data_flat_idx];
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

  std::vector<int> data_shape = data->get_shape();
  std::vector<int> indices_shape = indices->get_shape();
  std::vector<int> out_shape = out->get_shape();
  float* data_buffer = reinterpret_cast<float*>(data->get_buffer());
  float* indices_buffer = reinterpret_cast<float*>(indices->get_buffer());
  float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

  gather_func(data_buffer, indices_buffer, out_buffer, axis, indices_shape,
              data_shape, out_shape);

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_time - start_time;
  LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
  LOG(INFO) << "kernel run end";
}

void GatherCompute::set_params(Params* params) {
  params_ = reinterpret_cast<GatherParams*>(params);
}

GatherParams* GatherCompute::get_params() { return params_; }

REGISTER_KERNEL(GATHER, GatherCompute)