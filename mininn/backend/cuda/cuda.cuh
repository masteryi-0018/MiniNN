#ifndef __CUDA_CUH__
#define __CUDA_CUH__

void cuda_add_wrapper(const float* h_A, const float* h_B, float* h_C, int numElements);

#endif // __CUDA_CUH__