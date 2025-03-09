#ifndef USE_OP_H
#define USE_OP_H

#include "mininn/graph/register_helper.h"

// when you use any op, you should include this file
USE_OP(ADD);
USE_OP(CONV);
USE_OP(CLIP);
USE_OP(SHAPE);
USE_OP(GATHER);
USE_OP(UNSQUEEZE);
USE_OP(CONCAT);
USE_OP(GLOBALAVERAGEPOOL);
USE_OP(RESHAPE);
USE_OP(GEMM);
USE_OP(CONSTANT);

#endif // USE_OP_H