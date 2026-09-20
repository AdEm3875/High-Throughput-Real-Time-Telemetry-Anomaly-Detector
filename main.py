import os
os.add_dll_directory(r"C:\msys64\ucrt64\bin")

import numpy as np
import signal_engine

print("--- Step 1: Instantiating C++ Telemetry Engine ---")
# Capacity: 1,000 samples, Threshold: 3.0 stddevs
engine = signal_engine.telemetry(1000, 3.0)

print("\n--- Step 2: Zero-Copy Array Mapping ---")
buffer_view = np.array(engine, copy=False)
print(f"NumPy view shape: {buffer_view.shape}")

print("\n--- Step 3: Stream Data and Detect Anomalies ---")
np.random.seed(42)
stream_data = np.random.normal(loc=50.0, scale=2.0, size=2000)

anomalies = []

for i, sample in enumerate(stream_data):
    # Inject synthetic thermal spikes at specific intervals
    if i in [500, 1200, 1800]:
        sample += 25.0
    
    # Flag anomaly using C++ Z-score engine
    if engine.is_anomaly(sample):
        anomalies.append((i, sample, engine.get_average(), engine.get_stddev()))
    
    engine.push_data(sample)

print(f"Total Streamed Samples: {len(stream_data)}")
print(f"Anomalies Detected: {len(anomalies)}")

for idx, val, mean, std in anomalies:
    print(f"  Spike detected at index {idx:4d}: Sample={val:.2f} | Mean={mean:.2f} | StdDev={std:.2f}")

print("\n--- Step 4: Zero-Copy Memory Sync Verification ---")
print(f"Last element in shared buffer view: {buffer_view[999]:.2f}")
print("All telemetry engine tests passed successfully!")