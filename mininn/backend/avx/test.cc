#include <iostream>
#include <immintrin.h>  // AVX

// vectorAdd use avx/avx2
void avx_add_wrapper(const float* h_A, const float* h_B, float* h_C, int numElements) {
    size_t i = 0;
    // 32bit(float) * 8 = 256bit
    for (; i + 7 < numElements; i += 8) {
        // load 8 number of float32 to register
        __m256 vec_a = _mm256_loadu_ps(&h_A[i]);  // unaligned load
        __m256 vec_b = _mm256_loadu_ps(&h_B[i]);  // unaligned load

        // add
        __m256 vec_c = _mm256_add_ps(vec_a, vec_b);

        // store 8 number of float32 to register
        _mm256_storeu_ps(&h_C[i], vec_c);  // unaligned store
    }

    // remaining elements (less than 8)
    for (; i < numElements; ++i) {
        h_C[i] = h_A[i] + h_B[i];
    }
}

int main() {
    #ifdef __AVX__
    std::cout << "AVX is supported!" << std::endl;
    #else
    std::cout << "AVX is not supported." << std::endl;
    #endif

    #ifdef __AVX2__
    std::cout << "AVX2 is supported!" << std::endl;
    #else
    std::cout << "AVX2 is not supported." << std::endl;
    #endif

    #ifdef __AVX512F__
    std::cout << "AVX512F is supported." << std::endl;
    #else
    std::cout << "AVX512F is not supported." << std::endl;
    #endif

    #ifdef __AVX512VNNI__
    std::cout << "AVX512VNNI is supported." << std::endl;
    #else
    std::cout << "AVX512VNNI is not supported." << std::endl;
    #endif

    const size_t size = 500;
    float A[size], B[size], C[size];

    // initialize
    for (size_t i = 0; i < size; ++i) {
        A[i] = 1.0f;
        B[i] = 2.0f;
    }

    // vectorAdd use AVX
    avx_add_wrapper(A, B, C, size);

    // print
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "C[" << i << "]=" << C[i] << "  ";
    }
    std::cout << std::endl;

    return 0;
}