#include <iostream>
#include <xmmintrin.h>  // Include SSE instruction set
// #include <smmintrin.h> // Include SSE4.1/4.2 instruction set, but float compute is the same

// Vector addition function using SSE/SSE2
void sse_add_wrapper(const float* h_A, const float* h_B, float* h_C, int numElements) {
    size_t i = 0;
    // Process 4 floats at a time (SSE works with 128-bit registers, which hold 4 floats)
    for (; i + 3 < numElements; i += 4) {
        // Load 4 floats from arrays A and B into SSE registers
        __m128 vec_a = _mm_loadu_ps(&h_A[i]);
        __m128 vec_b = _mm_loadu_ps(&h_B[i]);
        
        // Perform element-wise addition
        __m128 vec_c = _mm_add_ps(vec_a, vec_b);
        
        // Store the result back to array C
        _mm_storeu_ps(&h_C[i], vec_c);
    }
    
    // Handle any remaining elements (if n is not a multiple of 4)
    for (; i < numElements; ++i) {
        h_C[i] = h_A[i] + h_B[i];
    }
}

int main() {
    #ifdef __SSE__
    std::cout << "SSE is supported!" << std::endl;
    #else
    std::cout << "SSE is not supported." << std::endl;
    #endif

    #ifdef __SSE2__
    std::cout << "SSE2 is supported!" << std::endl;
    #else
    std::cout << "SSE2 is not supported." << std::endl;
    #endif

    #ifdef __SSE3__
    std::cout << "SSE3 is supported." << std::endl;
    #else
    std::cout << "SSE3 is not supported." << std::endl;
    #endif

    #ifdef __SSSE3__
    std::cout << "SSSE3 is supported." << std::endl;
    #else
    std::cout << "SSSE3 is not supported." << std::endl;
    #endif

    #ifdef __SSE4_1__
    std::cout << "SSE4_1 is supported." << std::endl;
    #else
    std::cout << "SSE4_1 is not supported." << std::endl;
    #endif

    #ifdef __SSE4_2__
    std::cout << "SSE4_2 is supported." << std::endl;
    #else
    std::cout << "SSE4_2 is not supported." << std::endl;
    #endif

    const size_t size = 500;
    float A[size], B[size], C[size];

    // initialize
    for (size_t i = 0; i < size; ++i) {
        A[i] = 1.0f;
        B[i] = 2.0f;
    }

    // vectorAdd use AVX
    sse_add_wrapper(A, B, C, size);

    // print
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "C[" << i << "]=" << C[i] << "  ";
    }
    std::cout << std::endl;

    return 0;
}
