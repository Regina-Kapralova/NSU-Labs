#include <mpi.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;

double skalar(double* X, double* Y, const long M) {
    double R = 0, fullR = 0;
    for (long i = 0; i < M; ++i)
        R += X[i] * Y[i];
    MPI_Allreduce(&R, &fullR, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return fullR;
}

double* mul(double* A, double* X, const int size, const long M) {
    double* fullX = new double[size * M];
    MPI_Allgather(X, M, MPI_DOUBLE, fullX, M, MPI_DOUBLE, MPI_COMM_WORLD);

    double* Ax = new double[M];
    for (long i = 0; i < M; ++i) {
        Ax[i] = 0;
        for (long j = 0; j < size * M; ++j)
            Ax[i] += A[i * size * M + j] * fullX[j];
    }
    return Ax;
}

bool proverka(double* Ax, double* B, const long M) {
    double* Ax_b = new double[M];
    for (long i = 0; i < M; ++i)
        Ax_b[i] = Ax[i] - B[i];
    double u = 0, ub = 0;
    u = skalar(Ax_b, Ax_b, M);
    ub = skalar(B, B, M);
    delete[] Ax_b;
    if ((sqrt(u) / sqrt(ub)) < 0.00000001)
        return 0;
    else
        return 1;
}


double* find_x(double* A, double* B, double* X, const int size, const long M) {
    for (; ;) {
        MPI_Barrier(MPI_COMM_WORLD);
        double* Y = new double[M];
        double* Ax = mul(A, X, size, M);
        if (proverka(Ax, B, M) == 0)
            return X;
        for (long j = 0; j < M; ++j)
            Y[j] = Ax[j] - B[j];
        double* Ay = mul(A, Y, size, M);
        double t = 0.1 * (skalar(Y, Ay, M) / skalar(Ay, Ay, M));
        for (long j = 0; j < M; ++j)
            X[j] = X[j] - t * Y[j];
        delete[] Y;
        delete[] Ax;
        delete[] Ay;
    }
    return X;
}

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        cout << "Number of processes: " << size << endl;

    const long N = 4000, M = N / size;

    double* A = new double[N * M];
    for (long i = 0; i < M; ++i) {
        for (long j = 0; j < N; ++j){
            if (i + rank * M == j)
                A[i * N + j] = 2;
            else
                A[i * N + j] = 1;
        }
    }


    double* X = new double[M];
    double* U = new double[M];
    double* B = new double[M];
    for ( long i = 0; i < M; ++i) {
        X[i] = 0;
        B[i] = N + 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    X = find_x(A, B, X, size, M);
    double* fullX = new double[N];
    MPI_Allgather(X, M, MPI_DOUBLE, fullX, M, MPI_DOUBLE, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    double fulltime = 0;
    MPI_Allreduce(&time, &fulltime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    double resalt = 0;

    if (rank == 0) {
        for (int i = 0; i < N; ++i)
            resalt += fullX[i];
        cout << "Time taken: " << fulltime << " sec." << endl;
        cout << "Resalt = " << resalt << endl;
    }

    delete[] A;
    delete[] B;
    delete[] X;
    delete[] fullX;
    MPI_Finalize();
    return 0;
}
