#include "telemetry_engine.hpp"
#include <vector>
#include <cstddef>
#include <cmath>

telemetry::telemetry(std::size_t max, float threshold_)
    : max(max), ip(0), ts(0), sum(0.0), sum_sq_(0.0), threshold_(threshold_), vect(max, 0.0f) {}

void telemetry::push_data(float val) {
    if (ts >= max) {
        // Subtract overwritten values from running sums
        sum -= vect[ip];
        sum_sq_ -= (static_cast<double>(vect[ip]) * static_cast<double>(vect[ip]));
    }

    // Overwrite memory and add new sample to running sums
    vect[ip] = val;
    sum += val;
    sum_sq_ += (static_cast<double>(val) * static_cast<double>(val));

    // Advance write pointer and counter
    ip = (ip + 1) % max;
    ts++;
}

float telemetry::get_average() const {
    std::size_t active_count = (ts < max) ? ts : max;
    if (active_count == 0) {
        return 0.0f;
    }
    return static_cast<float>(sum / active_count);
}

float telemetry::get_stddev() const {
    std::size_t active_count = (ts < max) ? ts : max;
    if (active_count == 0) {
        return 0.0f;
    }

    double mean = sum / active_count;
    double variance = (sum_sq_ / active_count) - (mean * mean);

    if (variance <= 0.0) {
        return 0.0f;
    }

    return static_cast<float>(std::sqrt(variance));
}

bool telemetry::is_anomaly(float sample) const {
    float stddev = get_stddev();
    if (stddev <= 1e-6f) {
        return false;
    }

    float mean = get_average();
    float z_score = std::abs(sample - mean) / stddev;
    return z_score > threshold_;
}

py::buffer_info telemetry::get_buffer_info() {
    return py::buffer_info(
        vect.data(),                             // Memory pointer to raw vector
        sizeof(float),                            // Size of one float
        py::format_descriptor<float>::format(),   // Buffer format descriptor
        1,                                        // Dimensions
        { vect.size() },                          // Shape
        { sizeof(float) }                         // Strides
    );
}