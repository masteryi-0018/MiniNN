#ifndef CONV_H
#define CONV_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Conv: public Node {
public:
    Conv(Op type);

    ~Conv();

    void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_attributes(std::map<std::string, std::vector<int>>& attrs);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    ConvParams* params_;
};

#endif // CONV_H