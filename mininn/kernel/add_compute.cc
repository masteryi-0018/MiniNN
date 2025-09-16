#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h"

#include "mininn/backend/cuda/cuda.cuh"
#include "mininn/backend/opencl/opencl.h"
#include "mininn/backend/avx/avx.h"
#include "mininn/backend/sse/sse.h"
#include "mininn/backend/mkl/mkl.h"
#include "mininn/backend/neon/neon.h"

#include <thread>
#include <chrono>

AddCompute::AddCompute() {}

AddCompute::~AddCompute() {
    if (params_ != nullptr) {
        params_ = nullptr;
        delete params_;
    }
}

void add_func(float* x_buffer, float* y_buffer, float* out_buffer, int start, int end) {
    for (int i = start; i < end; ++i) {
        out_buffer[i] = x_buffer[i] + y_buffer[i];
    }
}

void add_func_multi_threads(float* x_buffer, float* y_buffer, float* out_buffer, int size) {
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int chunk_size = size / num_threads;

    for (size_t i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? size : start + chunk_size;
        threads.emplace_back(add_func, x_buffer, y_buffer, out_buffer, start, end);
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void AddCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    AddParams* params = get_params();
    std::shared_ptr<Tensor> x = params->input1;
    std::shared_ptr<Tensor> y = params->input2;
    std::shared_ptr<Tensor> out = params->output;

    int size = x->get_size();
    float* x_buffer = reinterpret_cast<float*>(x->get_buffer());
    float* y_buffer = reinterpret_cast<float*>(y->get_buffer());
    float* out_buffer = reinterpret_cast<float*>(out->get_buffer());

#ifdef WITH_MULTI_THREADS
    start_time = std::chrono::high_resolution_clock::now();
    add_func_multi_threads(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in multi_threads: " << elapsed_seconds.count() << " seconds";
#endif // WITH_MULTI_THREADS

#ifdef WITH_CUDA
    start_time = std::chrono::high_resolution_clock::now();
    cuda_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in cuda: " << elapsed_seconds.count() << " seconds";
#endif // WITH_CUDA

#ifdef WITH_OPENCL
    start_time = std::chrono::high_resolution_clock::now();
    opencl_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in opencl: " << elapsed_seconds.count() << " seconds";
#endif // WITH_OPENCL

#ifdef WITH_AVX
    start_time = std::chrono::high_resolution_clock::now();
    avx_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in avx: " << elapsed_seconds.count() << " seconds";
#endif // WITH_AVX

#ifdef WITH_SSE
    start_time = std::chrono::high_resolution_clock::now();
    sse_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in sse: " << elapsed_seconds.count() << " seconds";
#endif // WITH_SSE

#ifdef WITH_MKL
    start_time = std::chrono::high_resolution_clock::now();
    mkl_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in mkl: " << elapsed_seconds.count() << " seconds";
#endif // WITH_MKL

#ifdef WITH_NEON
    start_time = std::chrono::high_resolution_clock::now();
    neon_add_wrapper(x_buffer, y_buffer, out_buffer, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time in neon: " << elapsed_seconds.count() << " seconds";
#endif // WITH_NEON

    start_time = std::chrono::high_resolution_clock::now();
    add_func(x_buffer, y_buffer, out_buffer, 0, size);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";
}

void AddCompute::set_params(Params* params) {
    params_ = reinterpret_cast<AddParams*>(params);
}

AddParams* AddCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(ADD, AddCompute);