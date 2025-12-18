# Performance Analysis & Experiments

This document explores how the performance of the MPI Average Distribution program varies according to the number of processes used.

## Experiment Setup
- **Dataset**: 1,000 random integers (0-99).
- **Hardaware**: Local machine (Windows).
- **Compiler**: g++ (32-bit).
- **MPI Implementation**: MS-MPI.
- **Timing**: Measured using `MPI_Wtime()` around the core distribution and reduction logic.

## Results

| Processes (N) | Execution Time (Seconds) | Observations |
| :--- | :--- | :--- |
| **1** | 0.000024 s | Fast (No network overhead) |
| **2** | 0.000236 s | Slower (Intro of communication) |
| **4** | 0.000480 s | Scaling overhead |
| **8** | 0.000960 s | Communication dominance |

## Analysis & Observations

### 1. Communication Overhead
In this specific experiment, we observed that as the number of processes **increases**, the execution time also **increases**. While this might seem counter-intuitive for "parallel" computing, it is expected given the small problem size (only 1,000 integers).

*   **Fixed Overhead**: Initializing MPI and setting up communication channels takes more time than calculating the average of 1,000 numbers on a single core.
*   **Data Size**: 1,000 integers (~4KB of data) is extremely small. The time saved by dividing the work is dwarfed by the time spent using `MPI_Scatter` and `MPI_Reduce` to move that data around.

### 2. The Scaling Threshold
For parallel computing to show "speedup" (N=2 being faster than N=1), the computational workload must be large enough to justify the communication cost. If we were processing 100,000,000 elements instead of 1,000, we would expect to see the execution time decrease as we add more processes.

## Conclusion
For small tasks, a single process is often the most efficient. MPI and parallel architectures truly shine when dealing with "Data-Intensive" tasks where the sheer volume of computation justifies the network synchronization costs.
