#include <mpi.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;

double* mul_matrix(double *A, double *B, int N1, int N2, int N3){
    double* C = new double[N1 * N3];
    for (int i = 0; i < N1 * N3; i++)
        C[i] = 0;

    for (int i = 0; i < N1; i++)
        for (int k = 0; k < N3; k++)
            for (int j = 0; j < N2; j++)
                C[i*N3 + k] += A[i*N2 + j] * B[j*N3 + k];
    return C;
}

int main(int argc, char** argv) {

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[2] = {0, 0}, sizey, sizex; //sizey и sizex
    // определение размеров решетки: dims
    MPI_Dims_create(size, 2, dims);
    sizey = dims[0];
    sizex = dims[1];

    int periods[2] = {0,0}; //массив размера ndims для задания граничных условий
                    //(true - периодические, false - непериодические);
    int coords[2];
    int reorder = 1; //производить перенумерацию процессов (true) или нет (false);

    int ranky, rankx;

    // создание коммуникатора: comm2d
    MPI_Comm comm2d; // коммуникатор
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &comm2d);

    // получение своего номера в comm2d: rank
    MPI_Comm_rank(comm2d, &rank);

    // получение своих координат в двумерной решетке: coords
    MPI_Cart_get(comm2d, 2, dims, periods, coords);
    ranky = coords[0];
    rankx = coords[1];

    MPI_Comm col_comm, row_comm;
    int remain_dims[2];
    //коммутаторы для строк
    remain_dims[0] = 0;
    remain_dims[1] = 1;
    MPI_Cart_sub(comm2d, remain_dims, &row_comm);
    //коммутаторы для столбцов
    remain_dims[0] = 1;
    remain_dims[1] = 0;
    MPI_Cart_sub(comm2d, remain_dims, &col_comm);


    int N1 = 4000, N2 = 2000, N3 = 800;
    double* A;
    double* B;

    if (rank == 0) {
        A = new double[N1 * N2];
        B = new double[N2 * N3];
        for (int i = 0; i < N1 * N2; ++i)
            A[i] = i;
        for (int i = 0; i < N2 * N3; ++i)
            B[i] = 2*i;
    }

    int M1 = N1 / sizey;
    int M3 = N3 / sizex;
    double* part_A = new double[M1 * N2];
    double* part_B = new double[N2 * M3];

    MPI_Datatype col_vec, col_vec_B;
    MPI_Type_vector(N2, M3, N3, MPI_DOUBLE, &col_vec);
        //регистрируем новый тип данных
    MPI_Type_commit(&col_vec);
        //устанавливаем размер type_vector'а
    MPI_Type_create_resized(col_vec, 0, M3 * sizeof(double), &col_vec_B);
    MPI_Type_commit(&col_vec_B);

    if (rank % sizex == 0)
        MPI_Scatter(A, M1 * N2, MPI_DOUBLE, part_A, M1 * N2, MPI_DOUBLE, 0, col_comm);
    if (rank < sizex)
        MPI_Scatter(B, 1, col_vec_B, part_B, M3 * N2, MPI_DOUBLE, 0, row_comm);

    MPI_Bcast(part_A, M1 * N2, MPI_DOUBLE, 0, row_comm);
    MPI_Bcast(part_B, M3 * N2, MPI_DOUBLE, 0, col_comm);

    double* part_C = mul_matrix(part_A, part_B, M1, N2, M3);

    delete[] part_A;
    delete[] part_B;
    MPI_Type_free(&col_vec);
    MPI_Type_free(&col_vec_B);


    double* C;
    if (rank == 0)
        C = new double[N1 * N3];

    MPI_Datatype m_vector;
    MPI_Type_vector(M1, M3, N3, MPI_DOUBLE, &m_vector);
    MPI_Type_commit(&m_vector);

    MPI_Datatype m_vector_C;
    MPI_Type_create_resized(m_vector, 0, M3 * sizeof(double), &m_vector_C);
    MPI_Type_commit(&m_vector_C);

    int* counts = new int[size];
    int* displs = new int[size];
    for (int i = 0; i < sizey; i++)
        for (int j = 0; j < sizex; j++) {
            displs[i * sizex + j] = j + i * sizex * M1;
            counts[i * sizex + j] = 1;
        }

    MPI_Gatherv(part_C, M1 * M3, MPI_DOUBLE, C, counts , displs , m_vector_C, 0, comm2d);

    delete[] counts;
    delete[] displs;
    delete[] part_C;
    MPI_Type_free(&m_vector);
    MPI_Type_free(&m_vector_C);
    if (rank == 0) {
        delete[] A;
        delete[] B;
        delete[] C;
    }


    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double time = end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);
    double fulltime = 0;
    MPI_Allreduce(&time, &fulltime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "size = " << size << ", size_x = " << sizex << ", size_y = " << sizey << endl;
        cout << "	N1 = " << N1 << ", N2 = " << N2 << ", N3 = " << N3 << endl;
        cout << "		Time taken: " << fulltime << " sec." << endl << endl;
    }

    MPI_Finalize();
    return 0;
}
