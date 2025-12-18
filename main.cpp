/**
 * MPI Random Matrix Average Distribution Task
 * 
 * COMPATIBILITY NOTE: This block defines Microsoft SAL annotations
 * which are not natively supported by g++.
 */
#ifndef MSMPI_NO_SAL
#define MSMPI_NO_SAL
#endif
#ifndef _Out_writes_
#define _Out_writes_(x)
#endif
#ifndef _Inout_updates_
#define _Inout_updates_(x)
#endif
#ifndef _In_reads_
#define _In_reads_(x)
#endif
#ifndef _In_reads_opt_
#define _In_reads_opt_(x)
#endif
#ifndef _Inout_updates_opt_
#define _Inout_updates_opt_(x)
#endif
#ifndef _Out_writes_opt_
#define _Out_writes_opt_(x)
#endif
#ifndef _Out_writes_to_
#define _Out_writes_to_(x, y)
#endif
#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_(x)
#endif
#ifndef _In_reads_bytes_
#define _In_reads_bytes_(x)
#endif
#ifndef _In_
#define _In_
#endif
#ifndef _Out_
#define _Out_
#endif
#ifndef _In_opt_
#define _In_opt_
#endif
#ifndef _Inout_
#define _Inout_
#endif
#ifndef _Inout_opt_
#define _Inout_opt_
#endif
#ifndef _Ret_maybenull_
#define _Ret_maybenull_
#endif
#ifndef _Outptr_result_maybenull_
#define _Outptr_result_maybenull_
#endif
#ifndef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(x)
#endif
#ifndef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(x)
#endif
#ifndef _Outptr_
#define _Outptr_
#endif
#ifndef _Outptr_result_bytebuffer_
#define _Outptr_result_bytebuffer_(x)
#endif
#ifndef _In_range_
#define _In_range_(x, y)
#endif
#ifndef _Out_range_
#define _Out_range_(x, y)
#endif

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int TOTAL_ELEMENTS = 1000;
    
    // Calculate elements per process 
    // Assuming size divides 1000 evenly for simplicity as per user's last edit
    int elements_per_proc = TOTAL_ELEMENTS / size;
    
    std::vector<int> local_elements(elements_per_proc);
    std::vector<int> full_array;

    if (rank == 0) {
        full_array.resize(TOTAL_ELEMENTS);
        std::srand(std::time(0));
        std::cout << "[Rank 0] Generating " << TOTAL_ELEMENTS << " random elements..." << std::endl;
        for (int i = 0; i < TOTAL_ELEMENTS; ++i) {
            full_array[i] = std::rand() % 100;
        }
    }

    // Step 1: Distribute elements
    MPI_Scatter(rank == 0 ? full_array.data() : nullptr, 
                elements_per_proc, 
                MPI_INT,
                local_elements.data(), 
                elements_per_proc, 
                MPI_INT, 
                0, 
                MPI_COMM_WORLD);

    std::cout << "[Rank " << rank << "] received " << elements_per_proc << " elements. First 5: ";
    for (int i = 0; i < 5 && i < elements_per_proc; ++i) {
        std::cout << local_elements[i] << (i == 4 || i == elements_per_proc - 1 ? "" : ", ");
    }
    std::cout << " ... " << std::endl;

    // Step 2: Calculate local average
    long local_sum = 0;
    for (int val : local_elements) {
        local_sum += val;
    }
    double local_avg = (double)local_sum / elements_per_proc;

    std::cout << "[Rank " << rank << "] Local sum = " << local_sum 
              << ", Local Average = " << local_avg << std::endl;

    // Step 3: Calculate global average
    long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double global_avg = (double)global_sum / TOTAL_ELEMENTS;
        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "Global Average (Calculated at Rank 0) = " << global_avg << std::endl;
        std::cout << "Total elements processed = " << TOTAL_ELEMENTS << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
