# Performance Analysis & Experiments

This document explores how the performance of the MPI Average Distribution program scales with different dataset sizes and process counts.

## Experiment Setup
- **Dataset A**: 1,000 random integers (Small load).
- **Dataset B**: 1,000,000 random integers (Computational load).
- **Hardware**: Local machine (Windows).
- **Compiler**: g++ (32-bit).
- **MPI Implementation**: MS-MPI.
- **Timing**: Measured using `MPI_Wtime()` around distribution and reduction.

## Results: Dataset A (1,000 Elements)

| Processes (N) | Execution Time (Seconds) | Observations |
| :--- | :--- | :--- |
| **1** | 0.000016 s | Extremely fast, no communication. |
| **2** | 0.000238 s | Significant jump due to overhead. |
| **4** | 0.000522 s | Time continues to increase. |
| **8** | 0.000807 s | Communication overhead dominates. |

## Results: Dataset B (1,000,000 Elements)

| Processes (N) | Execution Time (Seconds) | Observations |
| :--- | :--- | :--- |
| **1** | 0.003364 s | Baseline for singular computation. |
| **2** | 0.002137 s | **Speedup observed!** Parallel gain > overhead. |
| **4** | 0.002116 s | Subtle improvement. |
| **8** | 0.002425 s | Diminishing returns (Parallel overhead sets in). |

## Analysis & Observations

### 1. The Threshold of Parallelism
Our experiments clearly highlight the **threshold of parallelism**:
- With **1,000 elements**, the amount of work per process is too small. The time spent setting up `MPI_Scatter` and `MPI_Reduce` is much greater than the time saved by dividing the sum calculation.
- With **1,000,000 elements**, the computational time becomes significant. Splitting the work between 2 processes reduced the time by ~36%, proving that the parallel gain finally outweighed the communication cost.

### 2. Diminishing Returns (Amdahl's Law)
In Dataset B, we see that moving from 2 to 8 processes didn't provide a linear speedup. In fact, N=8 was slightly slower than N=4. This is because:
- **Communication Cost**: As more processes are added, the complexity of the scatter/reduce network increases.
- **Problem Size**: 1,000,000 is large but still relatively small for 8 modern threads. The extra communication steps for 8 processes eventually negate the speed gained by finer-grained parallelization.

## Conclusion
Parallel computing is not a "magic bullet" for speed; it requires a balance between the **Computational Complexity** of the task and the **Communication Overhead** of the MPI network. For this assignment, the benefits of MPI are truly visible once the data size reaches the millions.
