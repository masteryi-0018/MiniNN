#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

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
    LOG("kernel run start");
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
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    LOG("kernel run end");


    // multi-threads
    LOG("kernel run start in multi-threads");
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
    std::cout << "num_threads: " << num_threads << std::endl;
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
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    LOG("kernel run end in multi-threads");
}

void AddCompute::set_params(Params* params) {
    params_ = reinterpret_cast<AddParams*>(params);
}

AddParams* AddCompute::get_params() {
    return params_;
}

REGISTER_KERNEL(ADD, AddCompute);