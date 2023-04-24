// Compile: mpicc int_ring.c -o int_ring
// Usage: mpiexec -np 4 ./int_ring 10
// the last parameter is optional (default value is 10) and could be other integers

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, N = 10, value = 0;
    double tt; // start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    tt = MPI_Wtime();
    for (int i = 0; i < N; i++) {
        if (rank == 0) {
            MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
        } else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
            MPI_Send(&value, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }
    tt = MPI_Wtime() - tt;
    if (rank == 0) {
        printf("Result after %d iterations: %d\n", N, value);
        printf("Time elapsed: %fs\n", tt);
    }
    MPI_Finalize();
    return 0;
}
