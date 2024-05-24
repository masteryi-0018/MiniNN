#ifndef ADD_H
#define ADD_H

#include "node.h"
#include "type.h"
#include "param.h"
#include "register.h"

#include <vector>

class Add : public Node {
public:

    Add();

    void set_kernel();

private:
    AddParams* params_;
};

#endif // ADD_H