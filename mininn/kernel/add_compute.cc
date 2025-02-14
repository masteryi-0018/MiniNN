#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

#include "mininn/backend/cuda/cuda.cuh"
#include "mininn/backend/opencl/opencl.h"
#include "mininn/backend/avx/avx.h"
#include "mininn/backend/sse/sse.h"

#include <thread>
#include <chrono>
#include <iostream> // todo

AddCompute::AddCompute() {}

void add_func(float* x_buffer, float* y_buffer, float* out_buffer, int start, int end) {
    for (int i = start; i < end; ++i) {
        out_buffer[i] = x_buffer[i] + y_buffer[i];
    }
}

void AddCompute::run() {
    LOG(INFO) << "kernel run start";
    auto start_time = std::chrono::high_resolution_clock::now();

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

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end";


    // multi-threads
    LOG(INFO) << "kernel run start in multi-threads";
    start_time = std::chrono::high_resolution_clock::now();

    params = get_params();
    x = params->input1;
    y = params->input2;
    out = params->output;

    size = x->get_size();
    x_buffer = reinterpret_cast<float*>(x->get_buffer());
    y_buffer = reinterpret_cast<float*>(y->get_buffer());
    out_buffer = reinterpret_cast<float*>(out->get_buffer());

    const size_t num_threads = std::thread::hardware_concurrency();
    LOG(INFO) << "num_threads: " << num_threads;
    std::vector<std::thread> threads;
    int chunk_size = size / num_threads;
    
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? size : start + chunk_size;
        threads.emplace_back(add_func, x_buffer, y_buffer, out_buffer, start, end);
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end in multi-threads";

    // cuda kernel
#ifdef __linux__
    // LOG(INFO) << "kernel run start in cuda kernel";
    // start_time = std::chrono::high_resolution_clock::now();

    // params = get_params();
    // x = params->input1;
    // y = params->input2;
    // out = params->output;

    // size = x->get_size();
    // x_buffer = reinterpret_cast<float*>(x->get_buffer());
    // y_buffer = reinterpret_cast<float*>(y->get_buffer());
    // out_buffer = reinterpret_cast<float*>(out->get_buffer());

    // cuda_add_wrapper(x_buffer, y_buffer, out_buffer, size);

    // end_time = std::chrono::high_resolution_clock::now();
    // elapsed_seconds = end_time - start_time;
    // LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    // LOG(INFO) << "kernel run end in cuda kernel";
#endif // __linux__

/*
    // opencl kernel
    LOG(INFO) << "kernel run start in opencl kernel";
    start_time = std::chrono::high_resolution_clock::now();

    params = get_params();
    x = params->input1;
    y = params->input2;
    out = params->output;

    size = x->get_size();
    x_buffer = reinterpret_cast<float*>(x->get_buffer());
    y_buffer = reinterpret_cast<float*>(y->get_buffer());
    out_buffer = reinterpret_cast<float*>(out->get_buffer());

    opencl_add_wrapper(x_buffer, y_buffer, out_buffer, size);

    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end in opencl kernel";
*/

    // avx kernel
    LOG(INFO) << "kernel run start in avx kernel";
    start_time = std::chrono::high_resolution_clock::now();

    params = get_params();
    x = params->input1;
    y = params->input2;
    out = params->output;

    size = x->get_size();
    x_buffer = reinterpret_cast<float*>(x->get_buffer());
    y_buffer = reinterpret_cast<float*>(y->get_buffer());
    out_buffer = reinterpret_cast<float*>(out->get_buffer());

    avx_add_wrapper(x_buffer, y_buffer, out_buffer, size);

    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end in avx kernel";

    // sse kernel
    LOG(INFO) << "kernel run start in sse kernel";
    start_time = std::chrono::high_resolution_clock::now();

    params = get_params();
    x = params->input1;
    y = params->input2;
    out = params->output;

    size = x->get_size();
    x_buffer = reinterpret_cast<float*>(x->get_buffer());
    y_buffer = reinterpret_cast<float*>(y->get_buffer());
    out_buffer = reinterpret_cast<float*>(out->get_buffer());

    sse_add_wrapper(x_buffer, y_buffer, out_buffer, size);

    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    LOG(INFO) << "Elapsed time: " << elapsed_seconds.count() << " seconds";
    LOG(INFO) << "kernel run end in sse kernel";
}

void AddCompute::set_params(Params* params) {
    params_ = reinterpret_cast<AddParams*>(params);
}

AddParams* AddCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(ADD, AddCompute);