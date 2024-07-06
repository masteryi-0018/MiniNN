#ifndef ADD_H
#define ADD_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Add: public Node {
public:
    Add(Op type);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    AddParams* params_;
};

#endif // ADD_H