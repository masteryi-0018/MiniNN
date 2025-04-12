#include "mininn/kernel/conv_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

ConvCompute::ConvCompute() {}

ConvCompute::~ConvCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void conv_func(float* input_buffer, float* weight_buffer, float* bias_buffer, float* out_buffer,
               std::vector<int> dilations, std::vector<int> group, std::vector<int> kernel_shape,
               std::vector<int> pads, std::vector<int> strides, std::vector<int> input_shape, std::vector<int> out_shape) {
    int batch_size = input_shape[0];
    int in_channels = input_shape[1];
    int input_height = input_shape[2];
    int input_width = input_shape[3];

    int out_channels = out_shape[1];
    int output_height = out_shape[2];
    int output_width = out_shape[3];

    int dilation_h = dilations[0];
    int dilation_w = dilations[1];

    int groups = group[0];
    int in_group_size = in_channels / groups;
    int out_group_size = out_channels / groups;

    int kernel_height = kernel_shape[0];
    int kernel_width = kernel_shape[1];

    int padding_left = pads[0];
    int padding_right = pads[1];
    int padding_up = pads[2];
    int padding_down = pads[3];

    int stride_height = strides[0];
    int stride_width = strides[1];

    // padding zero
    int padded_height = input_height + padding_up + padding_down;
    int padded_width = input_width + padding_left + padding_right;
    float* padded_input = new float[batch_size * in_channels * padded_height * padded_width];
    std::memset(padded_input, 0, batch_size * in_channels * padded_height * padded_width * sizeof(float));

    // cp input to padded_input
    for (int n = 0; n < batch_size; ++n) {
        for (int c = 0; c < in_channels; ++c) {
            for (int h = 0; h < input_height; ++h) {
                for (int w = 0; w < input_width; ++w) {
                    int padded_h_pos = h + padding_up;
                    int padded_w_pos = w + padding_left;
                    int src_idx = ((n * in_channels + c) * input_height + h) * input_width + w;
                    int dst_idx = ((n * in_channels + c) * padded_height + padded_h_pos) * padded_width + padded_w_pos;
                    padded_input[dst_idx] = input_buffer[src_idx];
                }
            }
        }
    }

    // conv2d
    for (int n = 0; n < batch_size; ++n) {
        for (int g = 0; g < groups; ++g) {
            for (int oc = 0; oc < out_group_size; ++oc) {
                int out_c = g * out_group_size + oc;
                for (int oh = 0; oh < output_height; ++oh) {
                    for (int ow = 0; ow < output_width; ++ow) {
                        float sum = bias_buffer[out_c];

                        for (int ic = 0; ic < in_group_size; ++ic) {
                            int in_c = g * in_group_size + ic;
                            for (int kh = 0; kh < kernel_height; ++kh) {
                                for (int kw = 0; kw < kernel_width; ++kw) {
                                    int in_h_pos = oh * stride_height + kh * dilation_h;
                                    int in_w_pos = ow * stride_width + kw * dilation_w;

                                    int input_idx = ((n * in_channels + in_c) * padded_height + in_h_pos) * padded_width + in_w_pos;
                                    int weight_idx = ((out_c * in_group_size + ic) * kernel_height + kh) * kernel_width + kw;
                                    sum += padded_input[input_idx] * weight_buffer[weight_idx];
                                }
                            }
                        }

                        int out_idx = ((n * out_channels + out_c) * output_height + oh) * output_width + ow;
                        out_buffer[out_idx] = sum;
                    }
                }
            }
        }
    }

    delete[] padded_input;
}

void ConvCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    ConvParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> weight = params->input2;
    std::shared_ptr<Tensor> bias = params->input3;
    std::shared_ptr<Tensor> out = params->output;
    std::vector<int> dilations = params->dilations;
    std::vector<int> group = params->group;
    std::vector<int> kernel_shape = params->kernel_shape;
    std::vector<int> pads = params->pads;
    std::vector<int> strides = params->strides;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* weight_buffer = reinterpret_cast<float*>(weight->get_buffer());
    float* bias_buffer = reinterpret_cast<float*>(bias->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    conv_func(input_buffer, weight_buffer, bias_buffer, out_buffer, 
              dilations, group, kernel_shape, pads, strides, input_shape, out_shape);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";
}

void ConvCompute::set_params(Params* params) {
    params_ = reinterpret_cast<ConvParams*>(params);
}

ConvParams* ConvCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(CONV, ConvCompute);