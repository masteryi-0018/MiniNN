#ifndef __METAL_H__
#define __METAL_H__

void metal_add_wrapper(const float* h_A, const float* h_B, float* h_C,
                       int numElements);

#endif  // __METAL_H__