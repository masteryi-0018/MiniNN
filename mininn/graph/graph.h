#ifndef GRAPH_H
#define GRAPH_H

#include "mininn/graph/node.h"
#include "mininn/graph/tensor.h"

#include <vector>
#include <memory>
#include <map> 

class Graph {
public:
    Graph();

    void add_node(std::shared_ptr<Node> node, std::vector<int>& inputs, std::vector<int>& outputs,
                  std::map<std::string, std::vector<int>>& attrs);

    std::vector<std::shared_ptr<Node>> get_nodes();

    void add_tensor(std::vector<int>& shape);

    std::vector<std::shared_ptr<Tensor>> get_tensors();
    
    void set_inputs(std::vector<int>& inputs);

    std::vector<int>& get_inputs();

    void set_outputs(std::vector<int>& outputs);

    std::vector<int>& get_outputs();

private:
    std::vector<std::shared_ptr<Node>> nodes_;
    std::vector<std::shared_ptr<Tensor>> tensors_;
    std::vector<int> inputs_;
    std::vector<int> outputs_;
};

#endif // GRAPH_H