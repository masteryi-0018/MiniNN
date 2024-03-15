#include <vector>

#include "node.h"
#include "tensor.h"

class Graph {
public:
    Graph(std::vector<Node*>& nodes, std::vector<Tensor*>& tensors);

    void add_node(Node* node);

    std::vector<Node*> get_nodes();

    std::vector<Tensor*> get_tensors();

    std::vector<int> get_inputs() const;

    void set_inputs(std::vector<int>& indices);

    std::vector<int> get_outputs() const;

    void set_outputs(std::vector<int>& indices);

private:
    std::vector<Node*> nodes_;
    std::vector<Tensor*> tensors_;
    std::vector<int> inputs_;
    std::vector<int> outputs_;
};