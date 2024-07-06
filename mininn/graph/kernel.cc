#include "mininn/graph/kernel.h"
#include "mininn/utils/log.h"

Kernel::Kernel() {}

// todo: when a func declared as a virtual func, should it be implement?
void Kernel::run() {}

void Kernel::set_params(Params* params) {
    LOG("set_params in base"); // todo: if don't add this, will segment fault
}

Params* Kernel::get_params() {
    return nullptr;
}