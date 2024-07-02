#ifndef NODE_H
#define NODE_H

#include "mininn/graph/type.h"
#include "mininn/graph/kernel.h"

#include <vector>
#include <memory>

class Node {
public:
    Node(Op type);

    void set_inputs(std::vector<int>& indices);

    std::vector<int> get_inputs() const;

    void set_outputs(std::vector<int>& indices);

    std::vector<int> get_outputs() const;

    std::shared_ptr<Kernel> create_kernel();

private:
    Op op_type_;
    std::vector<int> inputs_;
    std::vector<int> outputs_;
};

#endif // NODE_H