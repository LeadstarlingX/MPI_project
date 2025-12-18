# MPI Random Array Average Distribution

A C++ project using the Message Passing Interface (MPI) to distribute a random integer array across multiple processes, calculate local averages, and compute a global average.

## Project Overview

This project demonstrates a classic "Scatter-Reduce" pattern in parallel computing:
1.  **Generation**: Process 0 (Rank 0) generates 1,000 random integers.
2.  **Distribution**: The array is partitioned and distributed to all available processes using `MPI_Scatter`.
3.  **Local Computation**: Each process calculates the sum and average of its own subset of data.
4.  **Reduction**: All local sums are reduced to a global sum at Rank 0 using `MPI_Reduce`.
5.  **Final Result**: Rank 0 calculates and prints the final global average.

## Compatibility & Build Strategy

The project is specifically configured to work with the **g++** compiler on **Windows** using **Microsoft MPI**. 

*   **Header Fixes**: Includes a compatibility block in `main.cpp` to handle Microsoft's proprietary SAL annotations which usually cause errors in `g++`.
*   **32-bit Support**: Configured to link against the 32-bit (x86) MS-MPI libraries to match standard MinGW setups.

## Prerequisites

To build and run this project, you need:

1.  **MinGW (g++)**: A 32-bit C++ compiler.
2.  **Microsoft MPI (MS-MPI)**:
    *   Download both `msmpisetup.exe` (Runtime) and `msmpisdk.msi` (SDK).
    *   [Download Link (Official Microsoft Site)](https://www.microsoft.com/en-us/download/details.aspx?id=100593)

### 1. Configure Makefile Paths (Mandatory)
Before building, you must verify that the paths in the `Makefile` match your local installation of Microsoft MPI. 

Open the `Makefile` and update the following lines if necessary:
- `CXXFLAGS`: Ensure `-I` points to the `Include` directory of your MS-MPI SDK.
- `LDFLAGS`: Ensure `-L` points to the `Lib\x86` directory of your MS-MPI SDK.

> [!NOTE]
> The default paths in the Makefile are:
> - Include: `C:\Program Files (x86)\Microsoft SDKs\MPI\Include`
> - Lib (32-bit): `C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86`

### 2. Build and Run
Open a terminal in the project directory and run:

2.  **Execute**:
    Run with 4 processes (default configuration):
    ```bash
    mingw32-make run
    ```
    *Note: You can change the number of processes by running `mpiexec -n <number> .\main.exe` manually.*

3.  **Cleanup**:
    To remove the compiled executable:
    ```bash
    mingw32-make clean
    # OR
    .\clean.bat
    ```

## Troubleshooting & Compatibility

During development, we encountered a known compatibility issue between **g++** and **Microsoft MPI**. Microsoft's header files (`mpi.h`) use proprietary **SAL (Source Annotation Language)** keywords that standard compilers like `g++` do not recognize.

### Diagnostic Scripts
We included two scripts in the `troubleshooting/` directory to help verify your environment:

1.  **[test_compiler.cpp](file:///d:/CodingProjects/Antigravity/MPI/troubleshooting/test_compiler.cpp)**: A simple "Hello World" to confirm that your `g++` compiler is installed and working correctly.
2.  **[minimal_test.cpp](file:///d:/CodingProjects/Antigravity/MPI/troubleshooting/minimal_test.cpp)**: A bare-minimum MPI program used to test linkage and verify that the `MSMPI_NO_SAL` macro fix successfully bridges the compatibility gap.

If you encounter "unknown type" errors during compilation, first ensure that the `MSMPI_NO_SAL` macro is defined before including `mpi.h` (this is already handled in the main source and the provided Makefile).

## GitHub Short Description
> A C++ MPI project that distributes a random array among multiple processes to calculate local and global averages, featuring compatibility fixes for g++ on Windows.
