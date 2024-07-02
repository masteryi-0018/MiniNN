#include "mininn/kernel/add_compute.h"
#include "mininn/graph/register.h"

#include "mininn/utils/log.h" // todo

AddCompute::AddCompute() {}

void AddCompute::run() {
    LOG("kernel +++");
}

REGISTER_KERNEL(ADD, AddCompute);