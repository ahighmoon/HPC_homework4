#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <cassert>  


// Scan A array and write result into prefix_sum array;
// use long data type to avoid overflow
void scan_seq(long* prefix_sum, const long* A, long n) {
    if (n == 0) return;
    prefix_sum[0] = A[0];
    for (long i = 1; i < n; i++) {
        prefix_sum[i] = prefix_sum[i-1] + A[i];
    }
}

void scan_mpi(long* prefix_sum, const long* A, long n) {
    // prefix_sum = B1, A = recv, n = chunksize

    prefix_sum[0] = A[0]; // I change this code to include the 1st entry of A
    for (int j = 1; j < n; j++){
        prefix_sum[j] = prefix_sum[j - 1] + A[j]; // change here from A[j-1] to A[j] to include current position in A
    }
        
    correction[0] = 0;
    for (int i = 1; i < p; i++) {
        correction[i] = correction[i-1] + prefix_sum[i * chunksize - 1];
    }

    // use the offset array to update scanned result, parallel
    for (int i = 1; i < p; i++){
        long head = i * chunksize;
        #pragma omp parallel for num_threads(p)
        for (int j = head; j < std::min(head + chunksize, n); j++){
            prefix_sum[j] += correction[i];
        }
    }
    free(correction);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    long N = 100000000;
    long p = 4;
    int rank, size;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    // size of each chunk should be a long int, rounded up
    long chunksize = ceil(N / double(p));
    
    long* A = (long*) malloc(N * sizeof(long));
    long* B1 = (long*) malloc(chunksize * sizeof(long));
    long* recv = (long*) malloc(chunksize * sizeof(long));
    long* correction = (long*) malloc(p * sizeof(long));
    for (long i = 0; i < chunksize; i++) B1[i] = 0;
    for (long i = 0; i < chunksize; i++) recv[i] = 0;

    double tt; 
    if (rank == 0){
        long* B0 = (long*) malloc(N * sizeof(long));
        for (long i = 0; i < N; i++) A[i] = rand();
        for (long i = 0; i < N; i++) B0[i] = 0;
        
        tt = MPI_Wtime(); ;
        scan_seq(B0, A, N);
        printf("sequential-scan = %fs\n", MPI_Wtime() - tt);
        free(B0);
        tt = MPI_Wtime();
    }
    printf("rank = %d, B1[1] = %ld\n", rank, B1[1]);
    MPI_Scatter(A, chunksize, MPI_LONG, recv, chunksize, MPI_LONG, 0, comm);
    printf("rank = %d, B1[1] = %ld\n", rank, B1[1]);

    scan_mpi(B1, recv, chunksize);
    //printf("parallel-scan   = %fs\n", MPI_Wtime() - tt);


    //long err = 0;
    //for (long i = 0; i < N; i++) {
        //err = std::max(err, std::abs(B0[i] - B1[i]));

        // if (err != 0) {
        //   std::cout << "B0[" << i << "] = " << B0[i] << std::endl;
        //   std::cout << "B1[" << i << "] = " << B1[i] << std::endl;
        //   std::cout << "B0[" << i <<"] - B1[" << i << "] = " << B0[i] - B1[i] << std::endl;
        //   std::cout << "i = " << i << ", err = " << err << "\n";
        //   break;
        // }
        // assert (err == 0);
    //}
    //printf("error = %ld\n", err);

    free(A);
    free(B1);
    MPI_Finalize();
    return 0;


}
