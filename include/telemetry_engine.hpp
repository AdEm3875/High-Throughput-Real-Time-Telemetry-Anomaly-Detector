#pragma once
#include "pybind11/pybind11.h"
#include <vector>
#include <cstddef>
namespace py=pybind11;
class telemetry{
    private:
        std::size_t max;
        std::size_t ip;
        std::size_t ts;
        double sum;
        double sum_sq_;
        float threshold_;
        std::vector<float> vect;
    public:
        telemetry(std::size_t max,float threshold_);
        void push_data(float val);
        float get_average() const;
        bool is_anomaly(float sample) const;
        float get_stddev() const;
        py::buffer_info get_buffer_info();
        
    
};