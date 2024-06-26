#include "add.h"
#include "register.h"

Add::Add(Op type): Node(type) {}

void Add::set_kernel() {}

REGISTER_OP(ADD, Add);