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

    // ����������, � ����� ������ ��������� ������� (�������� ����� 2 ��������)
    int color = rank % 2;  // color == 0 ��� ������, color == 1 ��� ������
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);  // ����� �������� �� 2 ������

    printf("Process %d is in group %d\n", rank, color);

    if (color == 0) {
        // ������ 1: ����� �� ������ � �������������� ���������� ���������
        MPI_Comm_size(new_comm, &group_size);
        MPI_Comm_rank(new_comm, &new_rank);

        printf("Process %d (global rank %d) is in Ring group as local rank %d\n", rank, rank, new_rank);

        // �������� ���������� ���������
        int dims[] = { group_size };
        int periods[] = { 1 };  // ������������� ��������� (��������� ������)
        int reorder = 0;
        MPI_Cart_create(new_comm, 1, dims, periods, reorder, &comm_ring);

        // ���������� ������� �� ������
        int source, dest;
        MPI_Cart_shift(comm_ring, 0, 1, &source, &dest);

        // ������� �������� ���� ���� ��������-���������� � �������� ���� �� ���������
        int send_data = new_rank;
        int recv_data;
        MPI_Sendrecv(&send_data, 1, MPI_INT, dest, 0, &recv_data, 1, MPI_INT, source, 0, comm_ring, MPI_STATUS_IGNORE);

        printf("Ring group: Process %d sent %d to %d and received %d from %d\n", new_rank, send_data, dest, recv_data, source);

        MPI_Comm_free(&comm_ring);
    }
    else {
        // ������ 2: ������������ �� ����� "master-slave" (������)
        MPI_Comm_size(new_comm, &group_size);
        MPI_Comm_rank(new_comm, &new_rank);

        printf("Process %d (global rank %d) is in Star group as local rank %d\n", rank, rank, new_rank);

        // � ���� ������ ������ ������� ���������� ��������, � ��������� - ������
        int master = 0;
        if (new_rank == master) {
            // ������ ���������� ��������� ������� ������
            for (int i = 1; i < group_size; i++) {
                int send_data = 100 + i;  // ��������� ��������� ��� ������
                MPI_Send(&send_data, 1, MPI_INT, i, 0, new_comm);
                printf("Master %d sent %d to slave %d\n", new_rank, send_data, i);
            }

            // ������ �������� ���������� �� �������
            for (int i = 1; i < group_size; i++) {
                int recv_data;
                MPI_Recv(&recv_data, 1, MPI_INT, i, 0, new_comm, MPI_STATUS_IGNORE);
                printf("Master %d received %d from slave %d\n", new_rank, recv_data, i);
            }
        }
        else {
            // ����� �������� ��������� �� �������
            int recv_data;
            MPI_Recv(&recv_data, 1, MPI_INT, master, 0, new_comm, MPI_STATUS_IGNORE);
            printf("Slave %d received %d from master %d\n", new_rank, recv_data, master);

            // ����� ���������� ������� �������
            int result = recv_data * 2;  // ��������� ����������
            MPI_Send(&result, 1, MPI_INT, master, 0, new_comm);
            printf("Slave %d sent %d back to master %d\n", new_rank, result, master);
        }

        //MPI_Comm_free(&comm_star);
    }

    MPI_Comm_free(&new_comm);
    MPI_Finalize();
    return 0;
}
