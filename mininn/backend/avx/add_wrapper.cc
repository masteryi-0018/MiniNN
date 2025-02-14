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