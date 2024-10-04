#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#define MIN_LENGTH_FOR_KARATSUBA 4 // numbers shorter are multiplied by a 
//#define BASE 10 // notation




#define Gleb



#ifdef Zhenek
// Функция для сравнения элементов при сортировке
int cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100000; // Количество элементов в массиве
    const int blockSize = N / size; // Размер блока для каждого процесса
    const int remainder = N % size; // Остаток от деления

    // Вычисляем начальный и конечный индекс для каждого процесса
    int start = blockSize * rank + std::min(rank, remainder);
    int end = start + blockSize + (rank < remainder ? 1 : 0);

    // Генерируем случайные числа в промежутке от 0 до 100000
    srand(time(nullptr) + rank);
    int* data = new int[N];
    for (int i = 0; i < N; ++i) {
        data[i] = rand() % 100001;
    }

    // Выводим исходный массив на экран
    if (rank == 0) {
        std::cout << "Исходный массив: ";
        for (int i = 0; i < N; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    // Сортируем блоки данных каждого процесса
    qsort(data + start, end - start, sizeof(int), cmp);

    // Собираем отсортированные блоки данных
    int* sortedData = nullptr;
    if (rank == 0) {
        sortedData = new int[N];
    }

    MPI_Gather(data + start, end - start, MPI_INT, sortedData, end - start, MPI_INT, 0, MPI_COMM_WORLD);

    // Выводим отсортированный массив на экран
    if (rank == 0) {
        std::cout << "Отсортированный массив: ";
        for (int i = 0; i < N; ++i) {
            std::cout << sortedData[i] << " ";
        }
        std::cout << std::endl;
    }

    delete[] data;
    if (rank == 0) {
        delete[] sortedData;
    }

    MPI_Finalize();

    return 0;
}
#endif // Zhenek

#ifdef Gleb
typedef struct LONG_NUMBER longNum;
typedef unsigned long int lengthNum;

#define BASE 10
#define MIN_LENGTH_FOR_KARATSUBA 2

struct LONG_NUMBER {
    int* values; // array with number digits written in reverse order
    lengthNum length;
};

longNum longNumGen(int length) {
    longNum num;
    num.length = length;
    num.values = (int*)malloc(num.length * sizeof(int)); // Явное приведение типа

    for (lengthNum i = 0; i < num.length; i++)
        num.values[i] = rand() % 5;

    return num;
}

void longNumPrint(longNum num) {
    int a = 0;
    for (lengthNum i = num.length - 1; i > 0; i--) {
        if (num.values[i] && a == 0)
            a = 1;
        if (a)
            printf("%d", num.values[i]);
    }
    printf("%d", num.values[0]);
    puts("");
}

longNum longNumSum(longNum a, longNum b) {
    longNum c;

    if (a.length >= b.length) {
        c.length = a.length + 1;
        c.values = (int*)malloc(c.length * sizeof(int)); // Явное приведение типа

        c.values[a.length - 1] = a.values[a.length - 1];
        c.values[a.length] = 0;

        for (lengthNum i = 0; i < b.length; i++)
            c.values[i] = a.values[i] + b.values[i];
    }
    else {
        c.length = b.length + 1;
        c.values = (int*)malloc(c.length * sizeof(int)); // Явное приведение типа

        c.values[b.length - 1] = b.values[b.length - 1];
        c.values[b.length] = 0;

        for (lengthNum i = 0; i < a.length; i++)
            c.values[i] = a.values[i] + b.values[i];
    }

    return c;
}

longNum longNumSub(longNum a, longNum b) {
    if (a.length >= b.length) {
        for (lengthNum i = 0; i < b.length; i++)
            a.values[i] -= b.values[i];
        return a;
    }
    else {
        for (lengthNum i = 0; i < a.length; i++)
            b.values[i] -= a.values[i];
        return b;
    }
}

void normalization(longNum num) {
    for (lengthNum i = 0; i < num.length - 1; i++) {
        if (num.values[i] >= BASE) {
            int carryover = num.values[i] / BASE;
            num.values[i + 1] += carryover;
            num.values[i] -= carryover * BASE;
        }
        else if (num.values[i] < 0) {
            int carryover = (num.values[i] + 1) / BASE - 1;
            num.values[i + 1] += carryover;
            num.values[i] -= carryover * BASE;
        }
    }
}

longNum bringing(longNum num, lengthNum len) {
    longNum newNum;
    newNum.length = len;
    newNum.values = (int*)malloc(newNum.length * sizeof(int)); // Явное приведение типа

    for (int i = 0; i < num.length; i++)
        newNum.values[i] = num.values[i];

    for (int i = (int)num.length; i < newNum.length; i++)
        newNum.values[i] = 0;

    return newNum;
}

longNum karatsubaFunction(longNum a, longNum b) {
    if (a.length > b.length)
        b = bringing(b, a.length);
    else if (a.length < b.length)
        a = bringing(a, b.length);

    longNum c;
    c.length = a.length + b.length;
    c.values = (int*)malloc(c.length * sizeof(int)); // Явное приведение типа

    if (a.length < MIN_LENGTH_FOR_KARATSUBA) {
        memset(c.values, 0, c.length * sizeof(int));

        for (lengthNum i = 0; i < a.length; i++)
            for (lengthNum j = 0; j < b.length; j++)
                c.values[i + j] += a.values[i] * b.values[j];
    }
    else {
        longNum aPart1 = { a.values, (a.length + 1) / 2 };
        longNum aPart2 = { a.values + aPart1.length, a.length / 2 };
        longNum bPart1 = { b.values, (b.length + 1) / 2 };
        longNum bPart2 = { b.values + bPart1.length, b.length / 2 };

        longNum aPartsSum = longNumSum(aPart1, aPart2);
        normalization(aPartsSum);

        longNum bPartsSum = longNumSum(bPart1, bPart2);
        normalization(bPartsSum);

        longNum productSumsParts = karatsubaFunction(aPartsSum, bPartsSum);
        longNum productSignPart = karatsubaFunction(aPart1, bPart1);
        longNum productLeadPart = karatsubaFunction(aPart2, bPart2);

        longNum sumMidTerms = longNumSub(longNumSub(productSumsParts, productSignPart), productLeadPart);

        memcpy(c.values, productSignPart.values, productSignPart.length * sizeof(int));
        memcpy(c.values + productSignPart.length, productLeadPart.values, productLeadPart.length * sizeof(int));

        for (lengthNum i = 0; i < sumMidTerms.length; i++)
            c.values[aPart1.length + i] += sumMidTerms.values[i];
    }
    normalization(c);

    return c;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <length_of_long_number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    srand((int)time(NULL));

    int longNumLen = atoi(argv[1]);

    int size, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Group MPI_GROUP_ORIGINAL, MPI_GROUP_KARATSUBA;
    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_ORIGINAL);

    int* inGroup = (int*)malloc(size * sizeof(int)); // Динамическое выделение памяти для группы
    for (int i = 0; i < size; i++)
        inGroup[i] = i;

    MPI_Group_incl(MPI_GROUP_ORIGINAL, size, inGroup, &MPI_GROUP_KARATSUBA);
    MPI_Comm MPI_COMM_KARATSUBA;
    MPI_Comm_create(MPI_COMM_WORLD, MPI_GROUP_KARATSUBA, &MPI_COMM_KARATSUBA);

    for (int i = 0; i < size; i++) {
        if (rank == 0 && i == 0) {
            MPI_Datatype TYPE0;
            MPI_Type_contiguous(longNumLen, MPI_INT, &TYPE0);
            MPI_Type_commit(&TYPE0);

            longNum m_0 = longNumGen(longNumLen);
            printf("Process %d:\n m_0: ", rank);
            longNumPrint(m_0);

            MPI_Send(&m_0.length, 1, MPI_UNSIGNED_LONG, 1, 1, MPI_COMM_KARATSUBA);
            MPI_Send(m_0.values, 1, TYPE0, 1, 0, MPI_COMM_KARATSUBA);

            MPI_Type_free(&TYPE0);
        }
        else if (rank > 0 && rank == i) {
            lengthNum num = 0;
            MPI_Recv(&num, 1, MPI_UNSIGNED_LONG, i - 1, 1, MPI_COMM_KARATSUBA, &status);
            longNum recv_m_i = longNumGen((int)num);
            recv_m_i.length = num;

            MPI_Datatype TYPE_RECV_I;
            MPI_Type_contiguous((int)recv_m_i.length, MPI_INT, &TYPE_RECV_I);
            MPI_Type_commit(&TYPE_RECV_I);

            MPI_Recv(recv_m_i.values, 1, TYPE_RECV_I, i - 1, 0, MPI_COMM_KARATSUBA, &status);
            MPI_Type_free(&TYPE_RECV_I);

            printf("\nProcess %d:\n recv_m_%d: ", rank, status.MPI_SOURCE);
            longNumPrint(recv_m_i);

            longNum m_i = longNumGen(longNumLen);
            printf(" m_%d: ", rank);
            longNumPrint(m_i);

            longNum res_m = karatsubaFunction(m_i, recv_m_i);
            printf(" m_%d x m_%d: ", status.MPI_SOURCE, rank);
            longNumPrint(res_m);

            if (rank != (size - 1)) {
                MPI_Datatype TYPE_I;
                MPI_Type_contiguous((int)res_m.length, MPI_INT, &TYPE_I);
                MPI_Type_commit(&TYPE_I);

                MPI_Send(&res_m.length, 1, MPI_UNSIGNED_LONG, i + 1, 1, MPI_COMM_KARATSUBA);
                MPI_Send(res_m.values, 1, TYPE_I, i + 1, 0, MPI_COMM_KARATSUBA);

                MPI_Type_free(&TYPE_I);
            }
        }
    }

    MPI_Finalize();
    free(inGroup); // Освобождение памяти

    return 0;
}
#endif // Gleb
