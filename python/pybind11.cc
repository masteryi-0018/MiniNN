#include "mininn/graph/tensor.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(mininn, m) {
    py::class_<Tensor>(m, "Tensor")
        .def(py::init())
        .def("set_shape", &Tensor::set_shape)
        .def("get_shape", &Tensor::get_shape)
        .def("get_size", &Tensor::get_size)
        .def("get_buffer", &Tensor::get_buffer)
        .def("get_length", &Tensor::get_length);
}