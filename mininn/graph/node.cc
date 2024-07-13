#include "mininn/graph/node.h"
#include "mininn/graph/register.h"

Node::Node(Op type): op_type_(type) {}

std::shared_ptr<Kernel> Node::create_kernel() {
    std::shared_ptr<Kernel> kernel = KernelFactory::global().create(op_type_);
    init_kernel(kernel);
    return kernel;
}