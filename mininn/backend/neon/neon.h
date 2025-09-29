#ifndef __NEON_H__
#define __NEON_H__

void neon_add_wrapper(const float* h_A, const float* h_B, float* h_C,
                      int numElements);

#endif  // __NEON_H__