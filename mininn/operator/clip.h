#ifndef CLIP_H
#define CLIP_H

#include "mininn/graph/node.h"
#include "mininn/graph/type.h"
#include "mininn/graph/param.h"

class Clip: public Node {
public:
    Clip(Op type);

    ~Clip();

    void set_input_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_output_tensors(std::vector<std::shared_ptr<Tensor>>& tensors);

    void set_attributes(std::map<std::string, std::vector<int>>& attrs);

    void init_kernel(std::shared_ptr<Kernel> kernel);

private:
    ClipParams* params_;
};

#endif // CLIP_H