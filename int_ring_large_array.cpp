// mpic++ -o int_ring_large_array int_ring_large_array.cpp
// mpirun -np 4 ./int_ring_large_array 100
#include <iostream>
#include <mpi.h>
#include <vector>
#define ARRAY_SIZE  2*1024*1024/sizeof(int)

int main(int argc, char *argv[]) { //variable number, variable name 
    int rank, size, N = 10; //, value = 0; 
    double tt;
    int* value = (int*) malloc(ARRAY_SIZE*sizeof(int)); //initiate 2Mbyte array

    MPI_Init(&argc, &argv); // start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//MPI_COMM_WORLD communication within the world, rank-rank of processor
    MPI_Comm_size(MPI_COMM_WORLD, &size);//total number of processor 
    if (argc > 1) {
        N = std::stoi(argv[1]); // input argv[1] into N if input number is larger than 1, first is file name 
    }

    if (rank == 0) {
        // Initialize the array or perform any other computations
        for (size_t i = 0; i < ARRAY_SIZE; i++) {
            value[i] = i;
        }
    }

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Rank %d/%d running on %s.\n", rank, size, processor_name);

    tt = MPI_Wtime(); //test starttime, Wtime counts real time
    for (int i = 0; i < N; i++) {
        if (rank == 0)  {
            MPI_Send(value, ARRAY_SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            MPI_Recv(value, ARRAY_SIZE, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else {
            MPI_Recv(value, ARRAY_SIZE, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(value, ARRAY_SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }
    tt = MPI_Wtime() - tt;
    if (rank == 0) {
        double bandwidth = N*ARRAY_SIZE*sizeof(int)/tt/1e9; // in GB/s
        std::cout << "number of iterations: "  << N << std::endl;
        std::cout << "Time elapsed: " << tt << " seconds" << std::endl;
        std::cout << "Bandwidth: " << bandwidth << " GBytes/s" << std::endl;
        std::cout << "number of processes: " << size << std::endl;
    }
    MPI_Finalize();
    return 0;
}
