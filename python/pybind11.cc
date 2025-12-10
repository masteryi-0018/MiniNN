#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "mininn/graph/graph.h"
#include "mininn/graph/node.h"
#include "mininn/graph/register_helper.h"
#include "mininn/graph/tensor.h"
#include "mininn/graph/type.h"
#include "mininn/graph/use_kernel.h"
#include "mininn/graph/use_op.h"
#include "mininn/parser/parser.h"
#include "mininn/runtime/predictor.h"

namespace py = pybind11;

void bind_function(py::module_& m) { m.def("load_model", &load_model); }

void bind_enum(py::module_& m) {
  py::enum_<Op>(m, "Op").value("ADD", Op::ADD).export_values();
}

void bind_class(py::module_& m) {
  py::class_<Tensor, std::shared_ptr<Tensor>>(m, "Tensor")
      .def(py::init())
      .def("set_shape", &Tensor::set_shape)
      .def("get_shape", &Tensor::get_shape)
      .def("get_size", &Tensor::get_size)
      .def("get_buffer", &Tensor::get_buffer)
      .def("set_data", &Tensor::set_data)
      .def("get_data", &Tensor::get_data);

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

  py::class_<Predictor>(m, "Predictor")
      .def(py::init<std::shared_ptr<Graph>>())
      .def("prepare", &Predictor::prepare)
      .def("run", &Predictor::run)
      .def("async_run_future", &Predictor::async_run_future)
      .def("async_run_callback", &Predictor::async_run_callback)
      .def("get_input_tensors", &Predictor::get_input_tensors)
      .def("get_output_tensors", &Predictor::get_output_tensors)
      .def("dump_all_outputs", &Predictor::dump_all_outputs);
}

PYBIND11_MODULE(mininn_capi, m) {
  bind_function(m);
  bind_enum(m);
  bind_class(m);
}