#include <omp.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;

double* find_x(double* A, double* B, double* X, const long N) {
    bool flag = 0;

    double u = 0, ub = 0, Ay_y = 0, Ay_Ay = 0;
    double *Y, *Ax, *Ay, *Ax_b;

    #pragma omp parallel
    {
    while (flag == 0) {
       #pragma omp single
        {
            Y = new double[N];
            Ax = new double[N];
            Ay = new double[N];
            Ax_b = new double[N];
        }
        #pragma omp for
        for (long i = 0; i < N; ++i) {
            Ax[i] = 0;
            for (long j = 0; j < N; ++j)
                Ax[i] += A[i * N + j] * X[j];
        }

        /////////////  ртпчетлб
        #pragma omp for
        for (long i = 0; i < N; ++i)
            Ax_b[i] = Ax[i] - B[i];
        #pragma omp single
        {
        u = 0, ub = 0;
        }
        #pragma omp for reduction(+: u)
        for (long i = 0; i < N; ++i)
            u += Ax_b[i] * Ax_b[i];
        #pragma omp for reduction(+: ub)
        for (long i = 0; i < N; ++i)
            ub += B[i] * B[i];
        #pragma omp single
        {
        if ((sqrt(u) / sqrt(ub)) < 0.000000001)
            flag = 1;
        }
        /////////////

        #pragma omp for
        for (long j = 0; j < N; ++j)
            Y[j] = Ax[j] - B[j];
        #pragma omp for
        for (long i = 0; i < N; ++i) {
            Ay[i] = 0;
            for (long j = 0; j < N; ++j)
                Ay[i] += A[i * N + j] * Y[j];
        }
        #pragma omp single
        {
        Ay_y = 0, Ay_Ay = 0;
        }
        #pragma omp for reduction(+: Ay_y)
        for (long i = 0; i < N; ++i)
            Ay_y += Ay[i] * Y[i];
        #pragma omp for reduction(+: Ay_Ay)
        for (long i = 0; i < N; ++i)
            Ay_Ay += Ay[i] * Ay[i];

        double t;
        #pragma omp single
        {
         t = 0.05 * (Ay_y / Ay_Ay);
        }

        #pragma omp for
        for (long j = 0; j < N; ++j)
            X[j] = X[j] - t * Y[j];

        #pragma omp barrier
        #pragma omp single
        {
            delete[] Y;
            delete[] Ax;
            delete[] Ay;
            delete[] Ax_b;
        }
    }
    }
    return X;
}

int main(int argc, char** argv) {

    const long N = 8000;

    double* A = new double[N * N];
    for (long i = 0; i < N; ++i)
        for (long j = 0; j < N; ++j){
            if (i == j)
                A[i * N + j] = 2;
            else
                A[i * N + j] = 1;
        }

    double* X = new double[N];
    double* B = new double[N];
    for (long i = 0; i < N; ++i) {
        X[i] = 0;
        B[i] = N + 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    X = find_x(A, B, X, N);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);

    double resalt = 0;
    for (int i = 0; i < N; ++i)
        resalt += X[i];
    cout << "3) Time taken: " << time << " sec." << endl;
    cout << "Resalt = " << resalt << endl;

    delete[] A;
    delete[] B;
    delete[] X;
    return 0;
}
