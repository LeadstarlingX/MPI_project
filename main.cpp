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
    int elements_per_proc = TOTAL_ELEMENTS / size;

    // For simplicity in Step 1, assume size divides 1000 evenly.
    // We will handle remainders in the subsequent steps if needed.
    
    std::vector<int> local_elements(elements_per_proc);
    std::vector<int> full_array;

    if (rank == 0) {
        full_array.resize(TOTAL_ELEMENTS);
        std::srand(std::time(0));
        std::cout << "Rank 0: Generating 1000 random elements..." << std::endl;
        for (int i = 0; i < TOTAL_ELEMENTS; ++i) {
            full_array[i] = std::rand() % 100;
        }
    }

    // Step 1: Distribute elements
    MPI_Scatter(full_array.data(), elements_per_proc, MPI_INT,
                local_elements.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Rank " << rank << " received: ";
    for (int i = 0; i < elements_per_proc; ++i) {
        std::cout << local_elements[i] << (i == elements_per_proc - 1 ? "" : ", ");
    }
    std::cout << std::endl;

    // Step 2: Calculate local average
    long local_sum = 0;
    for (int val : local_elements) {
        local_sum += val;
    }
    double local_avg = (double)local_sum / elements_per_proc;

    std::cout << "Rank " << rank << ": Local sum = " << local_sum 
              << ", Local Average = " << local_avg << std::endl;

    // Step 3: Calculate global average
    // Note: The global average is global_sum / total_elements, 
    // NOT the average of the local averages.
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
