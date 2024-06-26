#ifndef ADD_H
#define ADD_H

#include "node.h"
#include "type.h"
#include "param.h"

class Add: public Node {
public:
    Add(Op type);

    void set_kernel();

private:
    AddParams* params_;
};

#endif // ADD_H