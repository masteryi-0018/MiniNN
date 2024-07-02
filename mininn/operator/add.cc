#include "mininn/operator/add.h"
#include "mininn/graph/register.h"

Add::Add(Op type): Node(type) {}

REGISTER_OP(ADD, Add);