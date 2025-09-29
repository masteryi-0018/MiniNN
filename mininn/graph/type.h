#ifndef OP_H
#define OP_H

enum Op {
  ADD = 0,
  CONV = 1,
  CLIP = 2,
  SHAPE = 3,
  GATHER = 4,
  UNSQUEEZE = 5,
  CONCAT = 6,
  GLOBALAVERAGEPOOL = 7,
  RESHAPE = 8,
  GEMM = 9,
  CONSTANT = 10,
};

#endif  // OP_H