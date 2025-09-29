#ifndef UNSQUEEZE_COMPUTE_H
#define UNSQUEEZE_COMPUTE_H

#include "mininn/graph/kernel.h"

class UnsqueezeCompute : public Kernel {
 public:
  UnsqueezeCompute();

  ~UnsqueezeCompute();

  void run();

  void set_params(Params* params);

  UnsqueezeParams* get_params();

 private:
  UnsqueezeParams* params_;
};

#endif  // UNSQUEEZE_COMPUTE_H