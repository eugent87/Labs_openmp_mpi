#include <iostream>
#include <mpi.h>
#define Gleb_3


using namespace std;

void num_3_Jenek(int N, int M) {
    printf("Num_1_jenek\n");
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < N + 1) {
        if (rank == 0) printf("������: ���������� ��������� ������ ���� �� ������ N+1\n");
        MPI_Finalize();
        exit(1);
    }

    for (int i = 0; i < M; i++) {
        int message;
        if (rank == 0) {
            // ������� ��������� ���������
            message = rand() % 100;
            cout << "����������� ������� ���������� ���������: " << message << endl;
        }

        // �������� ��������� ���� ���������
        MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0) {
            cout << "������� " << rank << " �������� ��������� �� ������������ ��������: " << message << endl;
        }

        // ��������� ������
        int response = rand() % 100;

        // �������� ������ �� ���� ���������
        int* responses = nullptr;
        if (rank == 0) {
            responses = new int[size];
        }

        MPI_Gather(&response, 1, MPI_INT, responses, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            for (int j = 1; j < size; j++) {
                cout << "����������� ������� �������� ����� �� �������� " << j << ": " << responses[j] << endl;
            }
            delete[] responses;
        }
    }

    MPI_Finalize();
}

void num_3_Gleb() {
    int rank, size;
    printf("num_3_Gleb\n");

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while (true) {
        int response = rand() % 21 - 10;  // ��������� ���������� ����� �� -10 �� 10

        if (rank == 0) {
            int* responses = new int[size];
            // �������� ������ �� ���� ���������
            MPI_Gather(&response, 1, MPI_INT, responses, 1, MPI_INT, 0, MPI_COMM_WORLD);

            for (int j = 1; j < size; j++) {
                cout << "����������� ������� �������� ����� �� �������� " << j << ": " << responses[j] << endl;
                if (responses[j] == -1) {  // ������� ������, ���� ������� -1
                    delete[] responses;
                    MPI_Finalize();
                    exit(0);
                }
            }
            delete[] responses;
        }
        else {
            // �������� ���������� ���� ������
            MPI_Gather(&response, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }
}


#ifdef Zhenek_3
int main(int argc, char** argv) {
    system("chcp 65001");
    MPI_Init(&argc, &argv);
    // ���������� ��������� ������ ���� ������ ����� ��������� ������
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " M N" << endl;
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);

    int M = atoi(argv[2]);

    num_3_Jenek(N, M);

    return 0;
}
#endif // DEBUG



#ifdef Gleb_3
int main(int argc, char** argv) {
    system("chcp 65001");
    MPI_Init(&argc, &argv);
    srand(time(NULL));

    num_3_Gleb();

    return 0;
}
#endif // Gleb_3
