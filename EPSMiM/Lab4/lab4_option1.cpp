#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>
#include <pthread.h>
#pragma warning(disable : 4996)

const int r = 4;
const int threads = 5;
pthread_t* thread;

double** U;
double* P;
const int Nx = 8000, Ny = 8000, Nt = 100;
const int Sx = 1, Sy = 1;
const double Xa = 0.0, Xb = 4.0, Ya = 0.0, Yb = 4.0;
double hx, hy;
double t;

const int ALIGN_INDENT = 64;
const int VECTOR_SIZE = 4;
double t2_arr[VECTOR_SIZE];
double d_x_arr[VECTOR_SIZE];
double d_y_arr[VECTOR_SIZE];
const double two_arr[VECTOR_SIZE] = {2.0, 2.0, 2.0, 2.0};

int* green;
int* red;
int* blue;

pthread_barrier_t barrier;

double f (int n) {
        double f0 = 1.0, t0 = 1.5, c = 4.0;
        double arg = 2*3.14*f0*(n*t - t0);
        return exp(-0.0625 * (arg*arg)) * sin(arg);
}

void calculateRow(int i, double *U_prv, double *U_new, double *P, double f_value) {

    double f_arr[VECTOR_SIZE];

    int row = i * Nx, row_ = (i - 1) * Nx, row1 = (i + 1) * Nx;

    for (int j = 1; j < Ny - 3; j += VECTOR_SIZE) {
            int rowcol = row + j, rowcol_ = row + j - 1, rowcol1 = row + j + 1,
                row_col = row_ + j, row1col = row1 + j, row_col_ = row_col - 1;

            if (i == Sx && (Sy - j) < VECTOR_SIZE && (Sy - j) >=0)
                f_arr[(Sy - j)] = f_value;
            else
                memset(f_arr, 0.0, 4 * sizeof(double));

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
//#define pthread_barrier_wait(x)

void one_thread(int thread_id){
    for (int n = 1; n < Nt; n += 4) {
        double* U_new = U[n%2];
        double* U_prv = U[(n+1)%2];
        double f_value = f(n);
        calculateRow(1, U_prv, U_new, P, f_value); //n=1 i=1
        calculateRow(2, U_prv, U_new, P, f_value); //n=1 i=2
        calculateRow(1, U_new, U_prv, P, f_value); //n=2 i=1
        calculateRow(3, U_prv, U_new, P, f_value); //n=1 i=3
        calculateRow(2, U_new, U_prv, P, f_value); //n=2 i=2
        calculateRow(1, U_prv, U_new, P, f_value); //n=3 i=1

        for (int i = 4; i < Ny - 1; i++) {
            calculateRow(i, U_prv, U_new, P, f_value);
            calculateRow(i-1, U_new, U_prv, P, f_value);
            calculateRow(i-2, U_prv, U_new, P, f_value);
            calculateRow(i-3, U_new, U_prv, P, f_value);
        }
        calculateRow(Ny-2, U_new, U_prv, P, f_value); //n=2 i = Ny-2
        calculateRow(Ny-3, U_prv, U_new, P, f_value); //n=3 i = Ny-3
        calculateRow(Ny-2, U_prv, U_new, P, f_value); //n=3 i = Ny-2
        calculateRow(Ny-4, U_new, U_prv, P, f_value); //n=4 i = Ny-4
        calculateRow(Ny-3, U_new, U_prv, P, f_value); //n=4 i = Ny-3
        calculateRow(Ny-2, U_new, U_prv, P, f_value); //n=4 i = Ny-2
    }
}

void first(int thread_id){
    for (int it = 1; it < Nt; it += r){
        int f_value = f(it);
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1), l = 1; l <= k; p--, l++)
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
        }
        for (int p = red[thread_id * 16]; p < green[(thread_id + 1) * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
                calculateRow(p - (l - 1), U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16] + (k - 2), l = k; l <= r; p--, l++) {
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
                calculateRow(p + 1, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void middle(int thread_id){
    for (int it = 1; it < Nt; it += r){
        int f_value = f(it);
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1) * 2, l = 1; l <= k; p--, l++) {
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
                calculateRow(p+1, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
            }
        }
        for (int p = red[thread_id * 16]; p < green[(thread_id + 1) * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
                calculateRow(p - (l - 1), U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16] + (k - 2), l = k; l <= r; p--, l++) {
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
                calculateRow(p + 1, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void last(int thread_id){
    for (int it = 1; it < Nt; it += r){
        int f_value = f(it);
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1) * 2, l = 1; l <= k; p--, l++) {
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
                calculateRow(p + 1, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
            }
        }
        for (int p = red[thread_id * 16]; p <= Nx - 2; p++)
        {
            for (int l = 1; l <= r; l++)
                calculateRow(p - (l - 1), U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16], l = k; l <= r; p--, l++)
                calculateRow(p, U[(it + l) % 2], U[(it + l - 1) % 2], P, f_value);
        }
        pthread_barrier_wait(&barrier);
    }
}

void* new_thread(void* param) {
    int thread_id = *((int*)param);
    free((int*)param);

    if (threads == 1)
        one_thread(thread_id);
    else
        if (thread_id == 0)
            first(thread_id);
    else
        if (thread_id == threads - 1)
            last(thread_id);
    else
        middle(thread_id);

    pthread_exit(NULL);
}

int main(){
    hx = (Xb - Xa) / (Nx - 1), hy = (Yb - Ya) / (Ny - 1);
        if (Nx <= 1000 && Ny <= 1000) t = 0.01;
        else t = 0.001;

    P = (double*)_mm_malloc(Nx * Ny * sizeof(double), ALIGN_INDENT);
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            if (j < Nx/2) P[i*Nx + j] = 0.1*0.1;
            else P[i*Nx + j] = 0.2*0.2;
    }

    U = (double**)_mm_malloc(2 * sizeof(double*), ALIGN_INDENT);
    U[0] = (double*)_mm_malloc(Nx * Ny * sizeof(double), ALIGN_INDENT);
    U[1] = (double*)_mm_malloc(Nx * Ny * sizeof(double), ALIGN_INDENT);
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nx; j++) {
            U[0][i*Nx + j] = 0.0;
            U[1][i*Nx + j] = 0.0;
    }

    double d_x = 0.5 / (hx * hx), d_y = 0.5 / (hy * hy), t2 = t * t;
    for (int i = 0; i < VECTOR_SIZE; i++) {
        t2_arr[i] = t2;
        d_x_arr[i] = d_x;
        d_y_arr[i] = d_y;
    }

        #ifdef pthread_barrier_wait
            printf("no barriers; ");
        #else
            printf("with barriers; ");
        #endif

        green = (int*)calloc(threads * 16, sizeof(int));
        red = (int*)calloc(threads * 16, sizeof(int));
        blue = (int*)calloc(threads * 16, sizeof(int));
        memset(green, 0, threads * 16*  sizeof(int));
        memset(red, 0, threads * 16 * sizeof(int));
        memset(blue, 0, threads * 16 * sizeof(int));

        for (int i = 0; i < threads; i++) {
            green[i * 16] = i * (Ny / threads) - (r - 1);
            red[i * 16] = i * (Ny / threads) + (r - 1);
            blue[i * 16] = (i + 1) * (Ny / threads) - r;
        }
        green[0] = 1;
        red[0]++;
        blue[(threads - 1) * 16] = Ny - r;

        pthread_barrier_init(&barrier, NULL, threads);

        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        thread = (pthread_t*)calloc(threads, sizeof(pthread_t));
        for (int i = 0; i < threads; i++) {
            int* thread_ID = (int*)malloc(sizeof(int));
            (*thread_ID) = i;
            int error_code = pthread_create(&thread[i], NULL, new_thread, thread_ID);
            if (error_code != 0)
                printf("[MAIN | Waring] Error in create thread.");
        }

        for (int i = 0; i < threads; i++)
            pthread_join(thread[i], NULL);

        clock_gettime(CLOCK_MONOTONIC_RAW, &finish);

        double time = finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec);
        printf("threads = %d : time = %lf\n", threads, time);
        FILE* file = fopen("file.dat", "wb");
        fwrite(U[1], sizeof(double), Nx * Ny, file);
        fclose(file);

        pthread_barrier_destroy(&barrier);
        free(green);
        free(red);
        free(blue);
        free(thread);
        free(U[0]);
        free(U[1]);
        free(P);

    return 0;
}
