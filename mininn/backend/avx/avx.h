#ifndef __AVX_H__
#define __AVX_H__

void avx_add_wrapper(const float* h_A, const float* h_B, float* h_C,
                     int numElements);

#endif  // __AVX_H__