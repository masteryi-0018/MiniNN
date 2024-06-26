#include "add_compute.h"
#include "register.h"
#include "log.h"

AddCompute::AddCompute() {}

void AddCompute::run() {
    LOG("kernel +++");
}

REGISTER_KERNEL(ADD, AddCompute);