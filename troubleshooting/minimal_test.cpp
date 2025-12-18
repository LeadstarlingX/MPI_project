/**
 * troubleshooting/minimal_test.cpp
 * 
 * Purpose: This script was used to diagnose the compatibility issue between 
 * g++ and Microsoft MPI. It tests if basic MPI initialization works and 
 * verifies the macro-based fix for Microsoft SAL annotations.
 */

#ifndef MSMPI_NO_SAL
#define MSMPI_NO_SAL
#endif
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "SUCCESS: MPI linkage and compatibility fix verified!" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
