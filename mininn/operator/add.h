#ifndef ADD_H
#define ADD_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"

class Add: public Node {
public:
    Add(Op type);
};

#endif // ADD_H