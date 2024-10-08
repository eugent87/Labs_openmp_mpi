#include <iostream>
#include <mpi.h>
#define num_1_jenek
using namespace std;


void num_1_Jenek(int N, int M) {
    printf("Num_1_jenek\n");
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < N + 1) {
        if (rank == 0) printf("Ошибка: количество процессов должно быть не меньше N+1\n");
        MPI_Finalize();
        exit(1);
    }

    for (int i = 0; i < M; i++) {
        if (rank == 0) {
            // Создать случайное сообщение
            int message = rand() % 100;
            
             cout << "Центральный процесс отправляет сообщение: " << message <<  endl;

            for (int j = 1; j <= N; j++) {
                MPI_Send(&message, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
            }

            int response;
            for (int j = 1; j <= N; j++) {
                MPI_Recv(&response, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                 cout << "Центральный процесс получает ответ от процесса " << j << ": " << response <<  endl;
            }
        }
        else {
            int message;
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
             cout << "процесс " << rank << " получает сообщение от центрального процесса: " << message <<  endl;

            
            int response = rand() % 100;


            MPI_Send(&response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}


void num_1_Gleb() {
    int rank, size,counter=0,max_value,j=0;
    printf("num_1_Gleb\n");
    

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    max_value = size;

   

    while (20)
    {
        if (rank == 0) {
            int response;
            while (true) {
                int response;
                // Выбор случайного процесса для получения 
                if (max_value > 1) { j = rand() % (max_value - 1) + 1; } // Исключаем ранг 0
                try {
                    MPI_Recv(&response, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    cout << "Центральный процесс получает ответ от процесса " << j << ": " << response << endl;
                }
                catch (exception ex) {
                    cout << ex.what() << endl;
                }

                // Если ответ -1, завершаем центральный процесс
                if (response == -1) {
                    MPI_Finalize();
                    exit(1);
                }
                else {
                    counter++;
                }
            }
}
        else {
            int response = rand() % 21 - 10;
            MPI_Send(&response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}


void num_2_Jenek(int N, int M) {
    printf("Num_2_jenek\n");
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < N + 1) {
        if (rank == 0) printf("Ошибка: количество процессов должно быть не меньше N+1\n");
        MPI_Finalize();
        exit(1);
    }

    for (int i = 0; i < M; i++) {
        int message;
        int* send_messages = nullptr;  // Массив для хранения сообщений
        if (rank == 0) {
            message = rand() % 100;
            cout << "Центральный процесс отправляет сообщение: " << message << endl;

            // Создание массива сообщений для каждого процесса
            send_messages = new int[size];
            for (int j = 0; j < size; j++) {
                send_messages[j] = message;
            }
        }

        // Каждому процессу отправляется своё сообщение (одинаковое в данном случае)
        MPI_Scatter(send_messages, 1, MPI_INT, &message, 1, MPI_INT, 0, MPI_COMM_WORLD);

        cout << "Процесс " << rank << " получает сообщение от центрального процесса: " << message << endl;

        int response = rand() % 100;

        int* responses = nullptr;
        if (rank == 0) {
            responses = new int[size];
        }

        // Собираем ответы от всех процессов
        MPI_Gather(&response, 1, MPI_INT, responses, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            for (int j = 1; j < size; j++) {
                cout << "Центральный процесс получает ответ от процесса " << j << ": " << responses[j] << endl;
            }
            delete[] responses;
            delete[] send_messages;
        }
    }

    MPI_Finalize();
}


void num_2_Gleb() {
    int rank, size, counter = 0, max_value, j = 0;
    printf("num_2_Gleb\n");

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    max_value = size;

    int* responses = nullptr;  // Массив для хранения ответов

    if (rank == 0) {
        // Выделяем память для массива только на центральном процессе
        responses = new int[size];
    }

    while (20)
    {
        if (rank == 0) {
            int response;
            while (true) {
                int response;
                // Выбор случайного процесса для получения 
                if (max_value > 1) { j = rand() % (max_value - 1) + 1; } // Исключаем ранг 0
                int* responses = nullptr;
                if (rank == 0) {
                    responses = new int[size];
                    MPI_Gather(&response, 1, MPI_INT, responses, 1, MPI_INT, 0, MPI_COMM_WORLD);
                }
                
                

                if (rank == 0) {
                    for (int j = 1; j < size; j++) {
                        if (responses[j] == -1) {
                            MPI_Finalize();
                            exit(1);
                        }
                        cout << "Центральный процесс получает ответ от процесса " << j << ": " << responses[j] << endl;

                    }
                    delete[] responses;
                    
                }
                   
                

                // Если ответ -1, завершаем центральный процесс
                if (response == -1) {
                    MPI_Finalize();
                    exit(1);
                }
                else {
                    counter++;
                }
            }
        }
        else {
            int response = rand() % 21 - 10;
            MPI_Send(&response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}




#ifdef num_1_jenek
int main(int argc, char** argv) {
    system("chcp 65001");
    MPI_Init(&argc, &argv);
    // Количество процессов должно быть задано через командную строку
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " M N" << endl;
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);

    int M = atoi(argv[2]);

    num_1_Jenek(N, M);

    return 0;
}
#endif //  mpiexec -n 5 C Users\User\source\repos\openmp\x64\Debug\MPI.exe 3 2


#ifdef num_1_gleb
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    srand(time(NULL));

    num_1_Gleb();

    return 0;
}
#endif // mpiexec -n 2 C:\Users\User\source\repos\openmp\x64\Debug\MPI.exe 


#ifdef num_2_jenek
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    srand(time(NULL));
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " M N" << endl;
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);

    int M = atoi(argv[2]);
    num_2_Jenek(N,M);

    return 0;
}
#endif // mpiexec -n 5 C:\Users\User\source\repos\openmp\x64\Debug\MPI.exe 3 2


#ifdef num_2_gleb
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    srand(time(NULL));
    
    num_2_Gleb();

    return 0;
}
#endif // mpiexec -n 5 C:\Users\User\source\repos\openmp\x64\Debug\MPI.exe 3 2