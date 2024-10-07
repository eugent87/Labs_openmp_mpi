#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int size, rank, new_rank, group_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm comm_ring, comm_star;
    MPI_Group world_group, ring_group, star_group;

    // Определяем, к какой группе относится процесс (чередуем между 2 группами)
    int color = rank % 2;  // color == 0 для кольца, color == 1 для звезды
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);  // Делим процессы на 2 группы

    printf("Process %d is in group %d\n", rank, color);

    if (color == 0) {
        // Группа 1: обмен по кольцу с использованием декартовой топологии
        MPI_Comm_size(new_comm, &group_size);
        MPI_Comm_rank(new_comm, &new_rank);

        printf("Process %d (global rank %d) is in Ring group as local rank %d\n", rank, rank, new_rank);

        // Создание декартовой топологии
        int dims[] = { group_size };
        int periods[] = { 1 };  // периодическое сообщение (замкнутое кольцо)
        int reorder = 0;
        MPI_Cart_create(new_comm, 1, dims, periods, reorder, &comm_ring);

        // Определяем соседей по кольцу
        int source, dest;
        MPI_Cart_shift(comm_ring, 0, 1, &source, &dest);

        // Процесс посылает свой ранг процессу-назначению и получает ранг от источника
        int send_data = new_rank;
        int recv_data;
        MPI_Sendrecv(&send_data, 1, MPI_INT, dest, 0, &recv_data, 1, MPI_INT, source, 0, comm_ring, MPI_STATUS_IGNORE);

        printf("Ring group: Process %d sent %d to %d and received %d from %d\n", new_rank, send_data, dest, recv_data, source);

        MPI_Comm_free(&comm_ring);
    }
    else {
        // Группа 2: коммуникация по схеме "master-slave" (звезда)
        MPI_Comm_size(new_comm, &group_size);
        MPI_Comm_rank(new_comm, &new_rank);

        printf("Process %d (global rank %d) is in Star group as local rank %d\n", rank, rank, new_rank);

        // В этой группе первый процесс становится мастером, а остальные - слейвы
        int master = 0;
        if (new_rank == master) {
            // Мастер отправляет сообщение каждому слейву
            for (int i = 1; i < group_size; i++) {
                int send_data = 100 + i;  // Некоторое сообщение для слейва
                MPI_Send(&send_data, 1, MPI_INT, i, 0, new_comm);
                printf("Master %d sent %d to slave %d\n", new_rank, send_data, i);
            }

            // Мастер получает результаты от слейвов
            for (int i = 1; i < group_size; i++) {
                int recv_data;
                MPI_Recv(&recv_data, 1, MPI_INT, i, 0, new_comm, MPI_STATUS_IGNORE);
                printf("Master %d received %d from slave %d\n", new_rank, recv_data, i);
            }
        }
        else {
            // Слейв получает сообщение от мастера
            int recv_data;
            MPI_Recv(&recv_data, 1, MPI_INT, master, 0, new_comm, MPI_STATUS_IGNORE);
            printf("Slave %d received %d from master %d\n", new_rank, recv_data, master);

            // Слейв отправляет обратно мастеру
            int result = recv_data * 2;  // Некоторое вычисление
            MPI_Send(&result, 1, MPI_INT, master, 0, new_comm);
            printf("Slave %d sent %d back to master %d\n", new_rank, result, master);
        }

        //MPI_Comm_free(&comm_star);
    }

    MPI_Comm_free(&new_comm);
    MPI_Finalize();
    return 0;
}
