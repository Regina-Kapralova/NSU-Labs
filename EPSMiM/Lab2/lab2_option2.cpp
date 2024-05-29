#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <xmmintrin.h>
#include <immintrin.h>
using namespace std;

int Nx = 8000, Ny = 8000, Nt = 100;
int Sx = 1, Sy = 1;
double t;
const int ALIGN_INDENT = 64;
const int VECTOR_SIZE = 4;

double f (int n) {
        double f0 = 1.0, t0 = 1.5, c = 4.0; // 1/c^2 = 0.0625
        double arg = 2*3.14*f0*(n*t - t0);
        return exp(-0.0625 * (arg*arg)) * sin(arg);
}

int main(int argc, char** argv) {
    for (int m = 0; m < 1; m++) {

    double Xa = 0.0, Ya = 0.0, Xb = 4.0, Yb = 4.0;
    double hx = (Xb - Xa) / (Nx - 1), hy = (Yb - Ya) / (Ny - 1);
        if (Nx <= 1000 && Ny <= 1000) t = 0.01;
        else t = 0.001;

    double* P = (double*)aligned_alloc(ALIGN_INDENT, Nx * Ny * sizeof(double));
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            if (j < Nx/2) P[i*Nx + j] = 0.1*0.1;
            else P[i*Nx + j] = 0.2*0.2;
    }

    double** U = (double**)malloc(2 * sizeof(double*));
    U[0] = (double*)aligned_alloc(ALIGN_INDENT, Nx * Ny * sizeof(double));
    U[1] = (double*)aligned_alloc(ALIGN_INDENT, Nx * Ny * sizeof(double));
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            U[0][i*Nx + j] = 0.0;
            U[1][i*Nx + j] = 0.0;
    }

    double d_x = 0.5 / (hx * hx), d_y = 0.5 / (hy * hy), t2 = t * t;
    double t2_arr[VECTOR_SIZE] = {t2, t2, t2, t2};
    double two_arr[VECTOR_SIZE] = {2.0, 2.0, 2.0, 2.0};
    double f_arr[VECTOR_SIZE];
    double d_x_arr[VECTOR_SIZE] = {d_x, d_x, d_x, d_x};
    double d_y_arr[VECTOR_SIZE] = {d_y, d_y, d_y, d_y};

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int n = 1; n < Nt; n++) {
        double* U_new = U[n%2];
        double* U_prv = U[(n+1)%2];
        double f_value = f(n);
        for (int i = 1; i < Ny-1; i++) {
            int row = i * Nx, row_ = (i - 1) * Nx, row1 = (i + 1) * Nx;
        for (int j = 1; j < Nx-1; j += VECTOR_SIZE) {
            int rowcol = row + j, rowcol_ = row + j - 1, rowcol1 = row + j + 1,
                                row_col = row_ + j, row1col = row1 + j, row_col_ = row_col - 1;
            if (i == Sx && (Sy - j) < VECTOR_SIZE && (Sy - j) >=0)
                f_arr[(Sy - j)] = f_value;
            else
                memset(f_arr, 0, 4 * sizeof(double));

            __m256d first = _mm256_loadu_pd(U_prv+rowcol1);
            __m256d second = _mm256_loadu_pd(U_prv+rowcol);
            __m256d res1 = _mm256_sub_pd(first, second);
            first = _mm256_loadu_pd(P+row_col);
            second = _mm256_loadu_pd(P+rowcol);
            __m256d res2 = _mm256_add_pd(first, second);
            __m256d firstPath = _mm256_mul_pd(res1, res2);

            first = _mm256_loadu_pd(U_prv+rowcol_);
            second = _mm256_loadu_pd(U_prv+rowcol);
            res1 = _mm256_sub_pd(first, second);
            first = _mm256_loadu_pd(P+row_col_);
            second = _mm256_loadu_pd(P+rowcol_);
            res2 = _mm256_add_pd(first, second);
            __m256d secondPath = _mm256_mul_pd(res1, res2);

            __m256d lineDividend = _mm256_add_pd(firstPath, secondPath);
            first = _mm256_loadu_pd(d_x_arr);
            __m256d firstLineResult = _mm256_mul_pd(lineDividend, first);

            first = _mm256_loadu_pd(U_prv+row1col);
            second = _mm256_loadu_pd(U_prv+rowcol);
            res1 = _mm256_sub_pd(first, second);
            first = _mm256_loadu_pd(P+rowcol_);
            second = _mm256_loadu_pd(P+rowcol);
            res2 = _mm256_add_pd(first, second);
            firstPath = _mm256_mul_pd(res1, res2);

            first = _mm256_loadu_pd(U_prv+row_col);
            second = _mm256_loadu_pd(U_prv+rowcol);
            res1 = _mm256_sub_pd(first, second);
            first = _mm256_loadu_pd(P+row_col_);
            second = _mm256_loadu_pd(P+row_col);
            res2 = _mm256_add_pd(first, second);
            secondPath = _mm256_mul_pd(res1, res2);

            lineDividend = _mm256_add_pd(firstPath, secondPath);
            first = _mm256_loadu_pd(d_y_arr);
            __m256d secondLineResult = _mm256_mul_pd(lineDividend, first);

            __m256d sumOfTwoLines = _mm256_add_pd(firstLineResult, secondLineResult);
            __m256d impulseVector = _mm256_loadu_pd(f_arr);
            __m256d bracketResult = _mm256_add_pd(sumOfTwoLines, impulseVector);


            first = _mm256_loadu_pd(two_arr);
            second = _mm256_loadu_pd(U_prv+rowcol);
            __m256d totalResult = _mm256_mul_pd(first, second);
            second = _mm256_loadu_pd(U_new+rowcol);
            totalResult = _mm256_sub_pd(totalResult, second);
            __m256d tauSquareVector = _mm256_loadu_pd(t2_arr);
            __m256d mulResult = _mm256_mul_pd(tauSquareVector, bracketResult);
            totalResult = _mm256_add_pd(totalResult, mulResult);
            _mm256_storeu_pd(U_new+rowcol, totalResult);
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    printf("%lf\n", time);
    FILE* file = fopen("file.dat", "wb");
    fwrite(U[1], sizeof(double), Nx * Ny, file);
    }

    return 0;
}
