#ifndef CLIP_COMPUTE_H
#define CLIP_COMPUTE_H

#include "mininn/graph/kernel.h"

// shared by ConvCompute for the optional fused clip
void clip_func(float* input_buffer, float* out_buffer, std::vector<int> max,
               std::vector<int> min, int size);

class ClipCompute : public Kernel {
 public:
  ClipCompute();

  ~ClipCompute();

  void run();

  void set_params(Params* params);

  ClipParams* get_params();

 private:
  ClipParams* params_;
};

#endif  // CLIP_COMPUTE_H