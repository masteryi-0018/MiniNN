#include <iostream>
#include <mkl.h>  // Include the MKL library header

// Vector addition function
void mkl_add_wrapper(const float* h_A, const float* h_B, float* h_C, int numElements) {
    // Use cblas_saxpy to perform vector addition C = A + B
    // cblas_saxpy(n, alpha, A, incA, B, incB)
    // alpha = 1.0, A and B are input vectors, C is the output vector
    // incA and incB are typically 1 (indicating stride), meaning elements are accessed sequentially
    for (int i = 0; i < numElements; i++) {
        h_C[i] = h_B[i];  // C starts as a copy of B
    }

    cblas_saxpy(numElements, 1.0f, h_A, 1, h_C, 1);  // C += A
}

