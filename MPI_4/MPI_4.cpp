#include <cstdio>
#include <mpi.h>
#include <cstddef>  
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 3


#define Gleb

using namespace std;
#ifdef Zhenek



#define MAX_POLY_SIZE 100

typedef struct {
    int degree;
    double coefficients[MAX_POLY_SIZE];
} Polynomial;

int main(int argc, char* argv[]) {
    system("chcp 65001");
    int rank, size;
    Polynomial polyA, polyB, result;
    MPI_Datatype polyType;
    MPI_Status status;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Определение типа MPI_Datatype для полинома
    int blocklengths[2] = { 1, MAX_POLY_SIZE };
    MPI_Datatype types[2] = { MPI_INT, MPI_DOUBLE };
    MPI_Aint displacements[2] = { offsetof(Polynomial, degree), offsetof(Polynomial, coefficients) };
    MPI_Type_create_struct(2, blocklengths, displacements, types, &polyType);
    MPI_Type_commit(&polyType);

    if (rank == 0) {
        // Инициализация полиномов A и B на процессе с рангом 0
        polyA.degree = 3;
        polyA.coefficients[0] = 2.0;
        polyA.coefficients[1] = 3.0;
        polyA.coefficients[2] = 1.0;
        polyA.coefficients[3] = 4.0;

        polyB.degree = 2;
        polyB.coefficients[0] = 1.0;
        polyB.coefficients[1] = 2.0;
        polyB.coefficients[2] = 3.0;

        // Отправка полиномов A и B другим процессам
        for (int i = 1; i < size; i++) {
            MPI_Send(&polyA, 1, polyType, i, 0, MPI_COMM_WORLD);
            MPI_Send(&polyB, 1, polyType, i, 1, MPI_COMM_WORLD);
        }
    }
    else {
        // Получение полиномов A и B на остальных процессах
        MPI_Recv(&polyA, 1, polyType, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&polyB, 1, polyType, 0, 1, MPI_COMM_WORLD, &status);
    }

    // Вычисление результатов произведения полиномов
    result.degree = polyA.degree + polyB.degree;
    for (int i = 0; i <= result.degree; i++) {
        result.coefficients[i] = 0.0;
    }

    for (int i = 0; i <= polyA.degree; i++) {
        for (int j = 0; j <= polyB.degree; j++) {
            result.coefficients[i + j] += polyA.coefficients[i] * polyB.coefficients[j];
        }
    }

    if (rank == 0) {
        // Получение результатов от остальных процессов
        Polynomial temp;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&temp, 1, polyType, i, 2, MPI_COMM_WORLD, &status);
            for (int j = 0; j <= temp.degree; j++) {
                result.coefficients[j] += temp.coefficients[j];
            }
        }

        // Вывод результата
        printf("Результат умножения полиномов:\n");
        for (int i = 0; i <= result.degree; i++) {
            printf("%.2f ", result.coefficients[i]);
        }
        printf("\n");
    }
    else {
        // Отправка результата на процесс с рангом 0
        MPI_Send(&result, 1, polyType, 0, 2, MPI_COMM_WORLD);
    }

    // Освобождение ресурсов MPI
    MPI_Type_free(&polyType);
    MPI_Finalize();

    return 0;
}
#endif // DEBUG

#ifdef Gleb
typedef struct COMPLEX complex;
typedef struct COMPLEX_MATRIX complex_matrix;

struct COMPLEX {
    double real;
    double imaginary;
};

struct COMPLEX_MATRIX {
    complex matrix[M][M];
};

complex complexGen(int i) {
    srand(i + (int)time(NULL));

    complex num;
    num.real = rand() % 11 - 5;
    num.imaginary = rand() % 11 - 5;

    return num;
}

complex_matrix matrixGen(int num) {
    complex_matrix arr;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++) {
            // provides random
            // sleep(i + j);
            arr.matrix[i][j] = complexGen(num + i + j);
        }
    return arr;
}

complex complexSum(complex a, complex b) {
    complex c;
    c.real = a.real + b.real;
    c.imaginary = a.imaginary + b.imaginary;
    return c;
}

complex complexMult(complex a, complex b) {
    complex c;
    c.real = (a.real * b.real - a.imaginary * b.imaginary);
    c.imaginary = (a.real * b.imaginary + a.imaginary * b.real);
    return c;
}

complex_matrix matrixMult(complex_matrix a, complex_matrix b) {
    complex_matrix c;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++) {
            c.matrix[i][j].real = 0;
            c.matrix[i][j].real = 0;
            for (int k = 0; k < M; k++)
                c.matrix[i][j] = complexSum(c.matrix[i][j], complexMult(a.matrix[i][k], b.matrix[k][j]));

        }
    return c;
}

void complexPrint(complex num) {
    if (num.imaginary >= 0)
        printf("%.1f + %.1fi\t", num.real, num.imaginary);
    else
        printf("%.1f - %.1fi\t", num.real, fabs(num.imaginary));
}

void matrixPrint(complex_matrix arr) {
    puts("");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++)
            complexPrint(arr.matrix[i][j]);
        puts("");
    }
    puts("");
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // create type for complex number
    MPI_Datatype _MPI_COMPLEX;
    MPI_Type_contiguous(2, MPI_DOUBLE, &_MPI_COMPLEX);
    MPI_Type_commit(&_MPI_COMPLEX);

    // create type for complex matrix
    MPI_Datatype MPI_COMPLEX_MATRIX;
    MPI_Type_contiguous(M * M, _MPI_COMPLEX, &MPI_COMPLEX_MATRIX);
    MPI_Type_commit(&MPI_COMPLEX_MATRIX);

    for (int i = 0; i < size; i++) {
        if (rank == 0 && i == 0) {
            complex_matrix m_0 = matrixGen(i);
            printf("Process %d:\nm_0: (%d)", rank, i);
            matrixPrint(m_0);

            MPI_Send(&m_0, 1, MPI_COMPLEX_MATRIX, 1, 0, MPI_COMM_WORLD);
        }
        else if (rank > 0 && rank == i) {
            complex_matrix recv_m_i;
            MPI_Recv(&recv_m_i, 1, MPI_COMPLEX_MATRIX, i - 1, 0, MPI_COMM_WORLD, &status);

            printf("Process %d:\nrecv_m_%d: (%d)", rank, status.MPI_SOURCE, i);
            matrixPrint(recv_m_i);

            complex_matrix m_i = matrixGen(i);
            printf("m_%d: (%d)", rank, i);
            matrixPrint(m_i);

            complex_matrix res_m = matrixMult(recv_m_i, m_i);
            printf("m_%d x m_%d: (%d)", status.MPI_SOURCE, rank, i);
            matrixPrint(res_m);

            if (rank != (size - 1))
                MPI_Send(&res_m, 1, MPI_COMPLEX_MATRIX, i + 1, 0, MPI_COMM_WORLD);
        }
    }

    // type invalidation
    MPI_Type_free(&_MPI_COMPLEX);
    MPI_Type_free(&MPI_COMPLEX_MATRIX);

    MPI_Finalize();

    return 0;
}
#endif // DEBUG