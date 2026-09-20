#include <pybind11/pybind11.h>
#include "telemetry_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(signal_engine, m) {
    m.doc() = "C++ Telemetry Anomaly Detector Engine";

    py::class_<telemetry>(m, "telemetry", py::buffer_protocol())
        .def(py::init<std::size_t, float>(), py::arg("max"), py::arg("threshold") = 3.0f)
        .def("push_data", &telemetry::push_data)
        .def("get_average", &telemetry::get_average)
        .def("get_stddev", &telemetry::get_stddev)
        .def("is_anomaly", &telemetry::is_anomaly)
        .def_buffer(&telemetry::get_buffer_info);
}