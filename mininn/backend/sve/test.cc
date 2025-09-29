#include <arm_sve.h>  // SVE intrinsics
#include <stdio.h>

void vec_add_sve_f32(const float* a, const float* b, float* c, int n) {
  int i = 0;
  int vl = svcntw();
  for (; i < n; i += vl) {
    svbool_t pg = svwhilelt_b32(i, n);
    svfloat32_t va = svld1(pg, a + i);
    svfloat32_t vb = svld1(pg, b + i);
    svfloat32_t vc = svadd_f32_m(pg, va, vb);
    svst1(pg, c + i, vc);
  }
}

int main() {
  float a[16], b[16], c[16];
  for (int i = 0; i < 16; i++) {
    a[i] = i;
    b[i] = 100 + i;
  }

  vec_add_sve_f32(a, b, c, 16);

  for (int i = 0; i < 16; i++) {
    printf("c[%d] = %f\n", i, c[i]);
  }
  return 0;
}
