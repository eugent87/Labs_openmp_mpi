#include <iostream>
#include <omp.h>
#include <vector>
#include <ctime>


using namespace std;


void lab_2() {
    const int NMAX = 800;
    const int LIMIT = 80;
    int i, j;
    float sum;
    float** a = new float* [NMAX];
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
};


void jenek_3_1() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel for reduction(+: result)
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;

    
};


void jenek_3_2() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }


    int result = 0;
    int value;
    double start_time = omp_get_wtime();
   

    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }

    
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения однопоток: " << (end_time - start_time) << " секунд" << endl;

};


void gleb_3_1() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 1;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel for reduction(*: result)
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i]!=0 and C[i]!=0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


};


void gleb_3_2() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 1;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с однопотоком: " << (end_time - start_time) << " секунд" << endl;


};


void jenek_4_atomic() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
    int itog_result = 0;
#pragma omp parallel 
    itog_result = 0;
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp atomic
    
        itog_result += result;
    
    
    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией atomic : " << (end_time - start_time) << " секунд" << endl;


};


void jenek_4_critical() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
    int itog_result = 0;
#pragma omp parallel 
     itog_result = 0;
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp critical
    {
        itog_result += result;
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией critical: " << (end_time - start_time) << " секунд" << endl;


};


void gleb_4_atomic() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
    int itog_result = 0;
#pragma omp parallel 
     itog_result = 0;
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp atomic
    
    itog_result += result;
    

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией atomic : " << (end_time - start_time) << " секунд" << endl;


};


void gleb_4_critical() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
    int itog_result = 0;
#pragma omp parallel 
     itog_result = 0;
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp critical
    {
        itog_result += result;
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией critical: " << (end_time - start_time) << " секунд" << endl;


};


void jenek_2_section() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel 

#pragma omp single
#pragma omp task
    for (int i = 0; i < N/2; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = N/2; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp taskwait

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


};


void jenek_4_section() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel
#pragma omp single
#pragma omp task
    for (int i = 0; i < N / 4; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = N / 4; i < N/2; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = N/2; i < 3*N / 4; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = 3*N/4; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                result += value;
            }
        }
    }
#pragma omp taskwait

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


};


void gleb_2_section() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel 
#pragma omp single
#pragma omp task
    for (int i = 0; i < N / 2; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = N / 2; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp taskwait

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


};


void gleb_4_section() {
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel

#pragma omp single
#pragma omp task
    for (int i = 0; i < N / 4; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = N / 4; i < N / 2; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
   
#pragma omp task
    for (int i = N / 2; i < 3 * N / 4; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp task
    for (int i = 3 * N / 4; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp taskwait

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


};



void zhenek_semaphore()
{
    omp_lock_t lock;
    omp_init_lock(&lock);
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
#pragma omp parallel default(none) shared(A, B, result, lock)
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            omp_set_lock(&lock);
            if (value != 1) {
                result += value;
            }
            omp_unset_lock(&lock);
        }

        else {
            int value = B[i] - A[i];
            omp_set_lock(&lock);
            if (value != 1) {
                result += value;
            }
            omp_unset_lock(&lock);
        }
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


}
void gleb_semaphore()
{
    omp_lock_t lock;
    omp_init_lock(&lock);
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 1;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
#pragma omp parallel default(none) shared(A, B, result, lock)
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            omp_set_lock(&lock);
            value = B[i] / C[i];
            result *= value;
            omp_unset_lock(&lock);
        }
        else {
            int value = B[i] + A[i];
            omp_set_lock(&lock);
            if (value != 0) {
                result += value;
            }
            omp_unset_lock(&lock);
        }
    }

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


}


void zhenek_barrier()
{
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0, thread_result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel default(none) shared(A, B, result)
#pragma omp for nowait
    for (int i = 0; i < N / 2; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                thread_result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                thread_result += value;
            }
        }
    }
#pragma omp for nowait
    for (int i = N / 2; i < N; ++i) {
        if (A[i] % 2 == 0) {
            value = B[i] + C[i];
            if (value != 1) {
                thread_result += value;
            }
        }
        else {
            int value = B[i] - A[i];
            if (value != 1) {
                thread_result += value;
            }
        }
    }

#pragma omp critical
    result += thread_result;

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;


}
void gleb_barrier()
{
    printf("Ведите размерность массива\n");
    int N;
    cin >> N;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    srand(time(0));

    // Инициализация массивов случайными числами
    for (int i = 0; i < N; ++i) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
        C[i] = rand() % 100;
    }

    int result = 0, thread_result = 0;
    int value;
    // Начало замера времени
    double start_time = omp_get_wtime();
    // Параллельная секция с редукцией
#pragma omp parallel default(none) shared(A, B, result)
#pragma omp for nowait
    for (int i = 0; i < N / 2; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }
#pragma omp for nowait
    for (int i = N / 2; i < N; ++i) {
        if (A[i] % 2 == 0 and B[i] != 0 and C[i] != 0) {
            value = B[i] / C[i];
            result *= value;
        }
        else {
            int value = B[i] + A[i];
            if (value != 0) {
                result += value;
            }
        }
    }

#pragma omp critical
    result += thread_result;

    // Окончание замера времени
    double end_time = omp_get_wtime();

    cout << "Результат: " << result << endl;
    cout << "Время выполнения с параллелизацией: " << (end_time - start_time) << " секунд" << endl;

}

void MEGAPROGA() {
    int qwest = 0;
    do {
        printf("                    ___________________________\n");
        printf("                    |Lab_2-------------------0|\n");
        printf("                    |JENia_3-1---------------1|\n");
        printf("                    |JENia_3-2---------------2|\n");//без паралельки
        printf("                    |Gleb_3-1----------------3|\n");
        printf("                    |Gleb_3-2----------------4|\n");//без паралельки
        printf("                    |Jenek_4_atomic----------5|\n");
        printf("                    |Gleb_4_atomic-----------6|\n");
        printf("                    |Jenek_4_critical--------7|\n");
        printf("                    |Gleb_4_critical---------8|\n");
        printf("                    |Jenek_2_section---------9|\n");
        printf("                    |Jenek_4_section--------10|\n");
        printf("                    |Gleb_2_section---------11|\n");
        printf("                    |Gleb_4_section---------12|\n");
        printf("                    |GLeb_2_semaphore-------13|\n");
        printf("                    |Gleb_2_barrier---------14|\n");
        printf("                    |Jenek_2_semaphore------15|\n");
        printf("                    |Jenek_2_barrier--------16|\n");
        printf("                    |cls--------------------17|\n");
        printf("                    |_________________________|\n");
        cin >> qwest;
        if (qwest < 0 or qwest > 17) {
            return;
        }
        switch (qwest)
        {
        case 0:
            lab_2();
            break;
        case 1:
            jenek_3_1();
            break;
        case 2:
            jenek_3_2();
            break;
        case 3:
            gleb_3_1();
            break;
        case 4:
            gleb_3_2();
            break;
        case 5:
            jenek_4_atomic();
            break;
        case 6:
            jenek_4_critical();
            break;
        case 7:
            gleb_4_atomic();
            break;
        case 8:
            gleb_4_critical();
            break;
        case 9:
            gleb_4_critical();
            break;
        case 10:
            gleb_4_critical();
            break;
        case 11:
            gleb_4_critical();
            break;
        case 12:
            gleb_4_critical();
            break;
        case 13:
            gleb_semaphore();
            break;
        case 14:
            gleb_barrier();
            break;
        case 15:
            zhenek_semaphore();
            break;
        case 16:
            zhenek_barrier();
            break;
        case 17:
            system("cls");
            break;
        }
        
    } while (true);
}

int main() {
    system("chcp 1251");
#ifdef _OPENMP
    MEGAPROGA();
#endif //_OPENMP


    return 0;
}
