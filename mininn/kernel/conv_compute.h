#ifndef CONV_COMPUTE_H
#define CONV_COMPUTE_H

#include "mininn/graph/kernel.h"

class ConvCompute : public Kernel {
 public:
  ConvCompute();

  ~ConvCompute();

  void run();

  void set_params(Params* params);

  ConvParams* get_params();

 private:
  ConvParams* params_;
};

#endif  // CONV_COMPUTE_H