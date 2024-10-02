#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

int ones_bin(int num) {
    int count = 0;
    while (num > 0) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}
int main1() {
    int M, N;

    cout << "Введите количество строк (M): ";
    cin >> M;
    cout << "Введите количество столбцов (N): ";
    cin >> N;

    srand(time(0));

    vector<vector<int>> matrix(M, vector<int>(N));

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 100 + 1;
        }
    }

    double start = clock();

    vector<int> sum_of_ones(M, 0);

    for (int i = 0; i < M; ++i) {
        int local_sum = 0;
        for (int j = 0; j < N; ++j) {
            for (int k = j + 1; k < N; ++k) {
                int proiz = matrix[i][j] * matrix[i][k];
                local_sum += ones_bin(proiz);
            }
        }
        sum_of_ones[i] = local_sum;
    }

    //for (int i = 0; i < M; ++i) {
    //    cout << "Сумма единиц в строке " << i + 1 << ": " << sum_of_ones[i] << endl;
    //}

    double end = clock();
    cout << "Время выполнения: " << (end - start) / CLOCKS_PER_SEC << " секунд" << endl;

    return 0;
}
