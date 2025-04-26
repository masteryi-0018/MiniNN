#include "mininn/kernel/gemm_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include <thread>
#include <chrono>
#include <iostream> // todo
#include <cstring> // memset

GemmCompute::GemmCompute() {}

GemmCompute::~GemmCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void gemm_func(float* input_buffer, float* weight_buffer, float* bias_buffer, float* out_buffer,
               std::vector<int> input_shape, std::vector<int> weight_shape, std::vector<int> bias_shape,
               std::vector<int> out_shape, std::vector<int> alpha, std::vector<int> beta, std::vector<int> transB) {
    // C = alpha * (A * B) + beta * C
    int M = input_shape[0];
    int K = input_shape[1];
    int N = weight_shape[transB[0] ? 0 : 1];

    float alpha_val = static_cast<float>(alpha[0]);
    float beta_val = static_cast<float>(beta[0]);
    bool transpose_B = transB[0] != 0;

    std::vector<float> temp_result(M * N, 0.0f);

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < K; ++k) {
                float a = input_buffer[i * K + k];
                float b = transpose_B ? weight_buffer[j * K + k] : weight_buffer[k * N + j];
                sum += a * b;
            }
            temp_result[i * N + j] = sum;
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float value = temp_result[i * N + j];

            value *= alpha_val;

            if (bias_buffer != nullptr && bias_shape.size() > 0) {
                if (bias_shape[0] == N) {
                    value += bias_buffer[j];
                } else if (bias_shape.size() == 2 && bias_shape[0] == M && bias_shape[1] == N) {
                    value += bias_buffer[i * N + j];
                }
            }

            out_buffer[i * N + j] = beta_val * out_buffer[i * N + j] + value;
        }
    }
}

void GemmCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

    GemmParams* params = get_params();
    std::shared_ptr<Tensor> input = params->input1;
    std::shared_ptr<Tensor> weight = params->input2;
    std::shared_ptr<Tensor> bias = params->input3;
    std::shared_ptr<Tensor> out = params->output;

    std::vector<int> alpha = params->alpha;
    std::vector<int> beta = params->beta;
    std::vector<int> transB = params->transB;

    std::vector<int> input_shape = input->get_shape();
    std::vector<int> weight_shape = weight->get_shape();
    std::vector<int> bias_shape = bias->get_shape();
    std::vector<int> out_shape = out->get_shape();
    float* input_buffer = reinterpret_cast<float*>(input->get_buffer());
    float* weight_buffer = reinterpret_cast<float*>(weight->get_buffer());
    float* bias_buffer = reinterpret_cast<float*>(bias->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

    gemm_func(input_buffer, weight_buffer, bias_buffer, out_buffer,
              input_shape, weight_shape, bias_shape, out_shape,
              alpha, beta, transB);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";
}

void GemmCompute::set_params(Params* params) {
    params_ = reinterpret_cast<GemmParams*>(params);
}

GemmParams* GemmCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(GEMM, GemmCompute);