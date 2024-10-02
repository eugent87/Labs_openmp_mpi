#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <Windows.h>


using namespace std;

void main(int  argc, char ** argv)
{
    int numtasks, rank=0;
    int mes = 0, procNum = 0;
    bool finished = false;
    MPI_Status status;
    srand(time(NULL));
    printf("enter qty tasks\n");
    cin >> numtasks;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // Главный процесс выводит количество задач
    if (rank == 0) {
       cout << "Num of all tasks: " << numtasks << endl;
    }

    // Цикл процессов
    while (!finished) {
        // Часть для главного процесса
        if (rank == 0) {
            static int counter = 0;
            MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // Если сообщение -1, завершить процесс
            if (mes == -1) {
                 cout << "Proc " << rank << " Get: " << mes << " From: " << status.MPI_SOURCE << " Counter: " << counter << " Proc finished" <<  endl;
                finished = true;

                // Отправить сигнал завершения другим процессам
                for (int i = 1; i < numtasks; i++)
                    MPI_Send(&mes, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            else {
                // Увеличить счётчик и показать значение
                counter++;
                 cout << "Proc " << rank << " Get: " << mes << " From: " << status.MPI_SOURCE << " Counter: " << counter <<  endl;
                MPI_Send(&mes, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
        }
        // Часть для других процессов
        else {
            mes = rand() % 10 - 1;
            MPI_Send(&mes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&mes, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (mes == -1) {
                finished = true;
                 cout << "Proc " << rank << " finished" <<  endl;
            }

#ifdef _WIN32
            Sleep(1000);  // Задержка на Windows
#else
            sleep(1);     // Задержка на Linux
#endif
        }
    }

    MPI_Finalize();

    // Освобождение памяти
    for (int i = 0; i < argc; i++) {
        delete[] argv[i];
    }
    delete[] argv;
}

//void MEGAPROGA() {
//    int qwest = 0;
//    do {
//        printf("                    ___________________________\n");
//        printf("                    |Lab_1_zhenek------------0|\n");
//        printf("                    |Lab_1_gleb--------------1|\n");
//        printf("                    |Lab_2_zhenek------------2|\n");
//        printf("                    |Lab_2_gleb--------------3|\n");
//        printf("                    |Lab_3_zhenek------------4|\n");
//        printf("                    |Lab_3_gleb==------------5|\n");
//        printf("                    |cls--------------------17|\n");
//        printf("                    |_________________________|\n");
//        cin >> qwest;
//        if (qwest < 0 or qwest > 17) {
//            return;
//        }
//        switch (qwest)
//        {
//        case 0:
//            break;
//        case 1:
//        }
//
//    } while (true);
//}

//int main()
//{
//
//    return 0;
//}