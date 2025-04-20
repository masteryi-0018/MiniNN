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
    for (uint32_t i = 0; i < tensors->size(); ++i) {
        auto fbs_tensor = tensors->Get(i);
        const flatbuffers::Vector<int32_t>* fbs_shape = fbs_tensor->shape();
        std::vector<int> shape;
        for (auto s : *fbs_shape) {
            shape.emplace_back(s);
        }

        const flatbuffers::Vector<uint8_t>* fbs_data = fbs_tensor->data();
        const void* raw_data = nullptr;
        if (fbs_data) {
            raw_data = reinterpret_cast<const void*>(fbs_data->data());
            
            // debug
            // size_t data_size = fbs_data->size();
            // const float* float_data = reinterpret_cast<const float*>(raw_data);
            // size_t num_elements = data_size / sizeof(float);

            // std::cout << "Data (float): " << num_elements << std::endl;
            // for (size_t i = 0; i < 10; i++) {
            //     std::cout << float_data[i] << " ";
            // }
            // std::cout << std::endl;
        }

        graph->add_tensor(shape, raw_data);
    }

    // nodes
    // consider use auto to handle fbs type
    const flatbuffers::Vector<flatbuffers::Offset<mininn_fbs::Node>>* nodes = fbs_graph->nodes();
    for (uint32_t i = 0; i < nodes->size(); ++i) {
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

        auto fbs_attrs = fbs_node->attributes();
        std::map<std::string, std::vector<int>> attrs;
        for (auto a : *fbs_attrs) {
            for (uint32_t i = 0; i < a->value()->size(); ++i) {
                attrs[a->key()->c_str()].emplace_back(a->value()->Get(i));
            }
        }

        graph->add_node(node, inputs, outputs, attrs);
    }

    return;
}