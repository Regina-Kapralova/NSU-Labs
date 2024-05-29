#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

int Nx = 8000, Ny = 8000, Nt = 100;
int Sx = 1, Sy = 1;
double t;

double f (int i, int j, int n) {
    if (j == Sx && i == Sy) {
        double f0 = 1.0, t0 = 1.5, c = 4.0 * 4.0; // 1/c^2 = 0.0625
        double arg = 2*3.14*f0*(n*t - t0);
        return exp(-0.0625 * arg * arg) * sin(arg);
    }
    else
        return 0.0;
}

int main(int argc, char** argv) {
    double Umax = 0;
    for (int m = 0; m < 5; m++) {

    double Xa = 0.0, Ya = 0.0, Xb = 4.0, Yb = 4.0;
    double hx = (Xb - Xa) / (Nx - 1), hy = (Yb - Ya) / (Ny - 1);
    if (Nx <= 1000 && Ny <= 1000)
        t = 0.01;
    else
        t = 0.001;

    double* P = (double*)malloc(sizeof(double) * Nx * Ny);
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            if (j < Nx/2)
                P[i*Nx + j] = 0.1*0.1;
            else
                P[i*Nx + j] = 0.2*0.2;
    }
    double** U = (double**)malloc(2 * sizeof(double*));
    U[0] = (double*)malloc(Nx * Ny * sizeof(double));
    U[1] = (double*)malloc(Nx * Ny * sizeof(double));
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            U[0][i*Nx + j] = 0.0;
            U[1][i*Nx + j] = 0.0;
    }
    double* Umax_n = (double*)malloc(sizeof(double) * Nt);
    for (int i = 0; i < Nt; i++)
        Umax_n[i] = 0.0;

    double d_x = 0.5 / (hx * hx), d_y = 0.5 / (hy * hy), t2 = t * t;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int n = 1; n < Nt; n++) {
        for (int i = 1; i < Ny-1; i++) {
        for (int j = 1; j < Nx-1; j++) {
            U[n%2][i*Nx + j] = 2*U[(n+1)%2][i*Nx + j] - U[n%2][i*Nx + j] + t*t*(f(i, j, n) +
                ((U[(n+1)%2][i*Nx + j+1]-U[(n+1)%2][i*Nx + j])*(P[(i-1)*Nx + j]+P[i*Nx + j]) +
                (U[(n+1)%2][i*Nx + j-1]-U[(n+1)%2][i*Nx + j])*(P[(i-1)*Nx + j-1]+P[i*Nx + j-1]) ) * d_x +
                ((U[(n+1)%2][(i+1)*Nx + j]-U[(n+1)%2][i*Nx + j])*(P[i*Nx + j-1]+P[i*Nx + j]) +
                (U[(n+1)%2][(i-1)*Nx+j]-U[(n+1)%2][i*Nx+j])*(P[(i-1)*Nx + j-1]+P[(i-1)*Nx + j]) ) * d_y);
            if (fabs(U[n%2][i*Nx + j]) > Umax_n[n])
                Umax_n[n] = fabs(U[n%2][i*Nx + j]);
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    printf("%lf\n", time);
    FILE* file = fopen("file.dat", "wb");
    fwrite(U[1], sizeof(double), Nx * Ny, file);
    for (int i = 0; i < Nt; i++)
        if (Umax_n[i] > Umax)
            Umax = Umax_n[i];
    }
    printf("\n %lf", Umax);

    return 0;
}
