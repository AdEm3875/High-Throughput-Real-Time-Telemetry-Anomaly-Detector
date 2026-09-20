
# Telemetry Anomaly Detector (C++ / Python)

A high-performance telemetry engine built in C++17 with Python bindings (`pybind11`). It processes streaming sensor data using a fixed-memory ring buffer, calculates $O(1)$ rolling statistics, and flags statistical anomalies using Z-scores—all mapped directly into Python/NumPy without copying memory.

---

## Key Features

* **Zero-Copy Memory Sharing:** Maps C++ heap memory directly into a NumPy array using Python's Buffer Protocol.
* **$O(1)$ Ring Buffer:** Keeps memory consumption completely flat by overwriting old samples in a fixed-capacity window (e.g., last 1,000 items).
* **Real-Time Anomaly Detection:** Updates rolling mean, variance, and standard deviation incrementally on every sample to flag outliers ($Z\text{-score} > \text{threshold}$).
* **Cross-Platform C++ Extension:** Built using CMake with static runtime linking to run cleanly without DLL errors.

---

## How It Works

1. **Fixed Memory Window:** As data streams in, samples populate a circular vector buffer. Once full, new samples overwrite the oldest entries.
2. **Online Math:** Running totals (`sum` and `sum_sq`) are updated in constant $O(1)$ time per sample rather than recalculating over the entire array.
3. **Z-Score Anomaly Trigger:** Each sample is evaluated against the current rolling statistics:

$$Z = \frac{\vert{}\text{sample} - \text{mean}\vert{}}{\text{stddev}}$$

If $Z$ exceeds the configured threshold (default: $3.0\sigma$), the sample is flagged as an anomaly.

---

## Project Structure

```text
telemetry-engine/
├── CMakeLists.txt             # Build configuration
├── include/
│   └── telemetry_engine.hpp   # Engine class header
├── src/
│   ├── telemetry_engine.cpp   # Ring buffer & math logic
│   └── bindings.cpp           # Pybind11 bindings
└── main.py                    # Python test driver script
Requirements
C++ Compiler: C++17 support (MinGW GCC, Clang, or MSVC)

CMake: Version 3.14 or higher

Python: 3.8 or higher

Python Libraries: numpy

Build & Run Instructions
1. Build the Extension Module
Open terminal in the project root folder:

PowerShell
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
This compiles signal_engine.pyd (Windows) or signal_engine.so (Linux/macOS).

2. Run the Diagnostic Test
Copy main.py into the build directory and run:

PowerShell
python main.py
Output Example
Plaintext
--- Step 1: Instantiating C++ Telemetry Engine ---

--- Step 2: Zero-Copy Array Mapping ---
NumPy view shape: (1000,)

--- Step 3: Stream Data and Detect Anomalies ---
Total Streamed Samples: 2000
Anomalies Detected: 8
  Spike detected at index    3: Sample=53.05 | Mean=50.67 | StdDev=0.68
  Spike detected at index   13: Sample=46.17 | Mean=50.58 | StdDev=1.37
  Spike detected at index  209: Sample=57.71 | Mean=49.94 | StdDev=1.85
  Spike detected at index  262: Sample=43.52 | Mean=50.01 | StdDev=1.94
  Spike detected at index  478: Sample=56.16 | Mean=50.02 | StdDev=1.94
  Spike detected at index  500: Sample=76.85 | Mean=50.01 | StdDev=1.96
  Spike detected at index 1200: Sample=75.25 | Mean=50.13 | StdDev=2.17
  Spike detected at index 1800: Sample=78.80 | Mean=50.21 | StdDev=2.13

--- Step 4: Zero-Copy Memory Sync Verification ---
Last element in shared buffer view: 48.51
All telemetry engine tests passed successfully!
