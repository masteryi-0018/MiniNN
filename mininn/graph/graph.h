#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "tensor.h"
#include "log.h"

#include <vector>
#include <memory>

class Graph {
public:
    Graph();

    void add_node(std::shared_ptr<Node> node, std::vector<int> inputs, std::vector<int> outputs);

    std::vector<std::shared_ptr<Node>> get_nodes();

    void add_tensor(Tensor* tensor);

    std::vector<Tensor*> get_tensors();

    std::vector<int> get_inputs() const;

    void set_inputs(std::vector<int>& indices);

    std::vector<int> get_outputs() const;

    void set_outputs(std::vector<int>& indices);

private:
    std::vector<std::shared_ptr<Node>> nodes_;
    std::vector<Tensor*> tensors_;
    std::vector<int> inputs_;
    std::vector<int> outputs_;
};

#endif // GRAPH_H