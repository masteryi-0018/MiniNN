#include <arm_neon.h>

void neon_add_wrapper(const float* h_A, const float* h_B, float* h_C,
                      int numElements) {
  int i = 0;
  // 4 elements at a time
  for (; i <= numElements - 4; i += 4) {
    float32x4_t va = vld1q_f32(h_A + i);
    float32x4_t vb = vld1q_f32(h_B + i);
    float32x4_t vc = vaddq_f32(va, vb);
    vst1q_f32(h_C + i, vc);
  }
  // remains
  for (; i < numElements; i++) {
    h_C[i] = h_A[i] + h_B[i];
  }
}