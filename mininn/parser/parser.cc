#include "mininn/parser/parser.h"
#include "mininn/parser/mininn_generated.h"
#include "mininn/graph/register.h"

#include <fstream> // std::ifstream
#include <iostream> // std::ios

void load_model(std::string& filename, std::shared_ptr<Graph> graph) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
    const mininn_fbs::Graph* fbs_graph = mininn_fbs::GetGraph(buffer.data());

    // inputs
    const flatbuffers::Vector<int32_t>* fbs_inputs = fbs_graph->inputs();
    std::vector<int> inputs;
    for (auto i : *fbs_inputs) {
        inputs.emplace_back(i);
    }
    graph->set_inputs(inputs);

    // outputs
    const flatbuffers::Vector<int32_t>* fbs_outputs = fbs_graph->outputs();
    std::vector<int> outputs;
    for (auto o : *fbs_outputs) {
        outputs.emplace_back(o);
    }
    graph->set_outputs(outputs);

    // tensors
    // consider use auto to handle fbs type
    const flatbuffers::Vector<flatbuffers::Offset<mininn_fbs::Tensor>>* tensors = fbs_graph->tensors();
    for (int i = 0; i < tensors->size(); ++i) {
        auto fbs_tensor = tensors->Get(i);
        const flatbuffers::Vector<int32_t>* fbs_shape = fbs_tensor->shape();
        std::vector<int> shape;
        for (auto s : *fbs_shape) {
            shape.emplace_back(s);
        }
        graph->add_tensor(shape);
    }

    // nodes
    // consider use auto to handle fbs type
    const flatbuffers::Vector<flatbuffers::Offset<mininn_fbs::Node>>* nodes = fbs_graph->nodes();
    for (int i = 0; i < nodes->size(); ++i) {
        auto fbs_node = nodes->Get(i);
        Op type = static_cast<Op>(fbs_node->type());
        std::shared_ptr<Node> node = OpFactory::global().create(type);

        auto fbs_inputs = fbs_node->inputs();
        std::vector<int> inputs;
        for (auto i : *fbs_inputs) {
            inputs.emplace_back(i);
        }

        auto fbs_outputs = fbs_node->outputs();
        std::vector<int> outputs;
        for (auto o : *fbs_outputs) {
            outputs.emplace_back(o);
        }

        graph->add_node(node, inputs, outputs);
    }

    return;
}