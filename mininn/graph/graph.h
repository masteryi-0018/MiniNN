#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "tensor.h"

#include <vector>
#include <memory>

class Graph {
public:
    Graph();

    void add_node(std::shared_ptr<Node> node, std::vector<int> inputs, std::vector<int> outputs);

    std::vector<std::shared_ptr<Node>> get_nodes();

    void add_tensor();

    std::vector<Tensor*> get_tensors();
    
    void set_inputs(std::vector<int>& indices);

    std::vector<int> get_inputs() const;

    void set_outputs(std::vector<int>& indices);

    std::vector<int> get_outputs() const;

    void prepare();

    std::vector<std::shared_ptr<Kernel>> get_kernels() const;

    void run();

private:
    std::vector<std::shared_ptr<Node>> nodes_;
    std::vector<Tensor*> tensors_;
    std::vector<int> inputs_;
    std::vector<int> outputs_;
    std::vector<std::shared_ptr<Kernel>> kernels_;
};

#endif // GRAPH_H