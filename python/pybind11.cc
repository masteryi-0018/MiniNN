#include "mininn/runtime/predictor.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(mininn, m) {
    py::class_<Predictor>(m, "Predictor")
        .def(py::init<std::shared_ptr<Graph>>(),
             py::arg("graph"))
        .def("prepare", &Predictor::prepare)
        .def("run", &Predictor::run)
        .def("async_run_future", &Predictor::async_run_future)
        .def("async_run_callback", &Predictor::async_run_callback)
        .def("get_input_tensors", &Predictor::get_input_tensors)
        .def("get_output_tensors", &Predictor::get_output_tensors);
}
