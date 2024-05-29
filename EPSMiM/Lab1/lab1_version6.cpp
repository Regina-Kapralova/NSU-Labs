#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

int Nx = 600, Ny = 600, Nt = 1000;
int Sx = 1, Sy = 1;
double t;

double f (int i, int j, int n) {
    if (j == Sx && i == Sy) {
        double f0 = 1.0, t0 = 1.5, c = 4.0; // 1/c^2 = 0.0625
        double arg = 2*3.14*f0*(n*t - t0);
        return exp(-0.0625 * (arg*arg)) * sin(arg);
    }
    else
        return 0.0;
}

int main(int argc, char** argv) {
    double Umax = 0;
    for (int m = 0; m < 1; m++) {

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
        double* U_new = U[n%2];
        double* U_prv = U[(n+1)%2];
        for (int i = 1; i < Ny-1; i++) {
            int row = i * Nx, row_ = (i - 1) * Nx, row1 = (i + 1) * Nx;
        for (int j = 1; j < Nx-1; j++) {
            int rowcol = row + j, rowcol_ = row + j - 1, rowcol1 = row + j + 1,
                                row_col = row_ + j, row1col = row1 + j, row_col_ = row_col - 1;
            U_new[rowcol] = 2*U_prv[rowcol] - U_new[rowcol] + t2*(f(i, j, n) +
                ((U_prv[rowcol1]-U_prv[rowcol])*(P[row_col]+P[rowcol]) +
                (U_prv[rowcol_]-U_prv[rowcol])*(P[row_col_]+P[rowcol_])) * d_x +
                ((U_prv[row1col]-U_prv[rowcol])*(P[rowcol_]+P[rowcol]) +
                (U_prv[row_col]-U_prv[rowcol])*(P[row_col_]+P[row_col])) * d_y);
            if (fabs(U_new[rowcol]) > Umax_n[n])
                Umax_n[n] = fabs(U_new[rowcol]);
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    printf("%lf\n", time);
    FILE* file = fopen("text.dat", "wb");
    fwrite(U[1], sizeof(double), Nx * Ny, file);
    for (int i = 0; i < Nt; i++)
        if (Umax_n[i] > Umax)
            Umax = Umax_n[i];
    }
    printf("\n %e", Umax);

    return 0;
}
