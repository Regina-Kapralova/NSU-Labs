#include <mpi.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <string.h>
using namespace std;

double realFunction(double x, double y, double z) {
    return x * x + y * y + z * z;
}

const double PARAMETER_A = 10e5;

double rightFunction(double x, double y, double z) {
    return 6 - PARAMETER_A * realFunction(x, y, z);
}

int main(int argc, char** argv) {

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int Nx = 1600;
    const int Ny = 100;
    const int Nz = 100;

    const double Hx = 2.0 / (Nx - 1);
    const double Hy = 2.0 / (Ny - 1);
    const double Hz = 2.0 / (Nz - 1);

    const double Hx2 = Hx * Hx;
    const double Hy2 = Hy * Hy;
    const double Hz2 = Hz * Hz;

    const double EPSILON = 10e-12;
    const double factor = 1 / (2 / Hx2 + 2 / Hy2 + 2 / Hz2 + PARAMETER_A);

    const int X = Nx / size;
    const int Y = Ny;
    const int Z = Nz;

    double* phi = new double[X * Y * Z];
    double* phiNext = new double[X * Y * Z];

    for (int i = 0; i < X; ++i)
        for (int j = 0; j < Y; j++)
            for (int k = 0; k < Z; k++) {
                if ((i + rank * X == 0) || (i + rank * X == Nx - 1)
                    || (j == 0) || (j == Ny - 1)
                    || (k == 0) || (k == Nz - 1)) {
                    phiNext[i * Y * Z + j * Z + k] = phi[i * Y * Z + j * Z + k] = realFunction((i + rank * (X - 1)) * Hx - 1, (j * Hy) - 1,
                        (k * Hz) - 1);
                }
                else
                    phiNext[i * Y * Z + j * Z + k] = phi[i * Y * Z + j * Z + k] = 0;
            }

    MPI_Request leftBorderSendRequest;
    MPI_Request rightBorderSendRequest;
    MPI_Request leftBorderRecvRequest;
    MPI_Request rightBorderRecvRequest;

    double* leftBorder = new double[Z * Y];
    double* rightBorder = new double[Z * Y];

    double maxDeltaPhi;
    double commonMaxDeltaPhi = 1;

    while (commonMaxDeltaPhi > EPSILON) {

        if (rank != size - 1) {
            MPI_Isend(&(phi[Y * Z * (X - 1) - 1]), Y * Z, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &rightBorderSendRequest);
            MPI_Irecv(rightBorder, Y * Z, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &rightBorderRecvRequest);
        }
        if (rank != 0) {
            MPI_Isend(&(phi[0]), Y * Z, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &leftBorderSendRequest);
            MPI_Irecv(leftBorder, Y * Z, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &leftBorderRecvRequest);
        }

        maxDeltaPhi = 0;
        for (int i = 1; i < X - 1; ++i)
            for (int j = 1; j < Y - 1; ++j)
                for (int k = 1; k < Z - 1; ++k) {
                    phiNext[i * Y * Z + j * Z + k] =
                        factor * ((phi[(i + 1) * Y * Z + j * Z + k] + phi[(i - 1) * Y * Z + j * Z + k]) / Hx2 +
                            (phi[i * Y * Z + (j + 1) * Z + k] + phi[i * Y * Z + (j - 1) * Z + k]) / Hy2 +
                            (phi[i * Y * Z + j * Z + (k + 1)] + phi[i * Y * Z + j * Z + (k - 1)]) / Hz2 -
                            rightFunction((i + rank * (X - 1)) * Hx - 1, j * Hy - 1, k * Hz - 1));

                    double deltaPhi = fabs(phiNext[i * Y * Z + j * Z + k] - phi[i * Y * Z + j * Z + k]);
                    if (deltaPhi > maxDeltaPhi)
                        maxDeltaPhi = deltaPhi;
                }

        if (rank != 0) {
            MPI_Wait(&leftBorderSendRequest, MPI_STATUS_IGNORE);
            MPI_Wait(&leftBorderRecvRequest, MPI_STATUS_IGNORE);
        }
        if (rank != size - 1) {
            MPI_Wait(&rightBorderSendRequest, MPI_STATUS_IGNORE);
            MPI_Wait(&rightBorderRecvRequest, MPI_STATUS_IGNORE);
        }

        for (int j = 1; j < Y - 1; ++j) {
            for (int k = 1; k < Z - 1; ++k) {
                if (rank != 0) {
                    int i = 0;
                    phiNext[i * Y * Z + j * Z + k] =
                        factor * ((phi[(i + 1) * Y * Z + j * Z + k] + leftBorder[j * Z + k]) / Hx2 +
                            (phi[i * Y * Z + (j + 1) * Z + k] + phi[i * Y * Z + (j - 1) * Z + k]) / Hy2 +
                            (phi[i * Y * Z + j * Z + (k + 1)] + phi[i * Y * Z + j * Z + (k - 1)]) / Hz2 -
                            rightFunction((i + rank * (X - 1)) * Hx - 1, j * Hy - 1, k * Hz - 1));

                    double deltaPhi = fabs(phiNext[i * Y * Z + j * Z + k] - phi[i * Y * Z + j * Z + k]);
                    if (deltaPhi > maxDeltaPhi)
                        maxDeltaPhi = deltaPhi;
                }
                if (rank != size - 1) {
                    int i = X - 1;
                    phiNext[i * Y * Z + j * Z + k] =
                        factor * ((rightBorder[j * Z + k] + phi[(i - 1) * Y * Z + j * Z + k]) / Hx2 +
                            (phi[i * Y * Z + (j + 1) * Z + k] + phi[i * Y * Z + (j - 1) * Z + k]) / Hy2 +
                            (phi[i * Y * Z + j * Z + (k + 1)] + phi[i * Y * Z + j * Z + (k - 1)]) / Hz2 -
                            rightFunction((i + rank * (X - 1)) * Hx - 1, j * Hy - 1, k * Hz - 1));

                    double deltaPhi = fabs(phiNext[i * Y * Z + j * Z + k] - phi[i * Y * Z + j * Z + k]);
                    if (deltaPhi > maxDeltaPhi)
                        maxDeltaPhi = deltaPhi;
                }
            }
        }
        memcpy(phi, phiNext, X * Y * Z * sizeof(double));
        MPI_Allreduce(&maxDeltaPhi, &commonMaxDeltaPhi, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    }

    double currMax = 0;
    for (int i = 1; i < X - 1; ++i)
        for (int j = 1; j < Y - 1; ++j)
            for (int k = 1; k < Z - 1; ++k) {
                double currDelta = fabs(
                    phi[i * Y * Z + j * Z + k] -
                    realFunction((i + rank * (X - 1)) * Hx - 1, j * Hy - 1, k * Hz - 1));
                if (currDelta > currMax)
                    currMax = currDelta;
            }
    double max;
    MPI_Allreduce(&currMax, &max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    double fulltime = 0;
    MPI_Allreduce(&time, &fulltime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Time taken: " << fulltime << endl;
        cout << "Difference between result and real function: " << max << endl;
    }
    delete[] phi;
    delete[] phiNext;
    delete[] rightBorder;
    delete[] leftBorder;

    MPI_Finalize();
    return 0;
}
