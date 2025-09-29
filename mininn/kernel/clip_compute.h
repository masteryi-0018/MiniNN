#ifndef CLIP_COMPUTE_H
#define CLIP_COMPUTE_H

#include "mininn/graph/kernel.h"

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