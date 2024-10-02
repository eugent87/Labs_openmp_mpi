#include <omp.h>
#include <iostream>
#include <ctime>


const int NMAX = 800;  // Установите значения для эксперимента
const int LIMIT = 801;  // Установите значения для эксперимента

int main() {
    system("chcp 1251");
    int i, j;
    float sum;
    float** a = new float*[NMAX];
    for (int i = 0;i < NMAX;i++) {
        a[i] = new float[NMAX];
    }

    // Инициализация массива
    for (i = 0; i < NMAX; i++)
        for (j = 0; j < NMAX; j++)
            a[i][j] = i + j;

    // Замер времени выполнения
    double start_time = omp_get_wtime();

#pragma omp parallel shared(a) if (NMAX > LIMIT)
    {
#pragma omp for private(i, j, sum)
        for (i = 0; i < NMAX; i++) {
            sum = 0;
            for (j = 0; j < NMAX; j++)
                sum += a[i][j];
#pragma omp critical
            std::cout << "Сумма элементов строки " << i << " равна " << sum << std::endl;
        }
    }

    // Замер окончания времени
    double end_time = omp_get_wtime();
    double exec_time = end_time - start_time;
    std::cout << "Время выполнения: " << exec_time << " секунд" << std::endl;

    return 0;
}
