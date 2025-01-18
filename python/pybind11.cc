#include "mininn/parser/parser.h"
#include "mininn/graph/type.h"
#include "mininn/graph/tensor.h"
#include "mininn/graph/node.h"
#include "mininn/graph/graph.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(mininn, m) {
    m.def("load_model", &load_model);

    py::enum_<Op>(m, "Op")
        .value("ADD", Op::ADD)
        .export_values();

    py::class_<Tensor, std::shared_ptr<Tensor>>(m, "Tensor")
        .def(py::init())
        .def("set_shape", &Tensor::set_shape)
        .def("get_shape", &Tensor::get_shape)
        .def("get_size", &Tensor::get_size)
        .def("get_buffer", &Tensor::get_buffer)
        .def("get_length", &Tensor::get_length);

    py::class_<Node, std::shared_ptr<Node>>(m, "Node");

    py::class_<Graph, std::shared_ptr<Graph>>(m, "Graph")
        .def(py::init())
        .def("add_node", &Graph::add_node)
        .def("get_nodes", &Graph::get_nodes)
        .def("add_tensor", &Graph::add_tensor)
        .def("get_tensors", &Graph::get_tensors)
        .def("set_inputs", &Graph::set_inputs)
        .def("get_inputs", &Graph::get_inputs)
        .def("set_outputs", &Graph::set_outputs)
        .def("get_outputs", &Graph::get_outputs);
}
