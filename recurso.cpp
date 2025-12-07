#include <iostream>
#include <vector>
#include "recurso.h"

using namespace std;

pair<vector<int>, double> recursoGreedy(vector<vector<double>>& tabla, int R) {
    int M = tabla[0].size();
    vector<int> asign(M, 0);

    while (R > 0) {
        double mejor = -1e9;
        int mejorJ = -1;

        for (int j = 0; j < M; j++) {
            double gain = tabla[asign[j] + 1][j] - tabla[asign[j]][j];
            if (gain > mejor) {
                mejor = gain;
                mejorJ = j;
            }
        }

        asign[mejorJ]++;
        R--;
    }

    double total = 0;
    for (int j = 0; j < M; j++)
        total += tabla[asign[j]][j];

    return {asign, total};
}

void menuRecurso() {
    int N, M, R;

    cout << "Ingrese N (max unidades): ";
    cin >> N;

    cout << "Ingrese M (elementos): ";
    cin >> M;

    vector<vector<double>> tabla(N+1, vector<double>(M));

    cout << "Ingrese la tabla (N+1)xM:\n";
    for (int i = 0; i <= N; i++)
        for (int j = 0; j < M; j++)
            cin >> tabla[i][j];

    cout << "Ingrese R: ";
    cin >> R;

    int op;
    while (true) {
        cout << "\n--- Distribucion de Recurso ---\n";
        cout << "1) Greedy\n";
        cout << "0) Volver\n";
        cin >> op;

        if (op == 1) {
            auto r = recursoGreedy(tabla, R);
            cout << "Asignaciones:\n";
            for (int i = 0; i < r.first.size(); i++)
                cout << "Elemento " << i << ": " << r.first[i] << "\n";
            cout << "Total: " << r.second << "\n";
        }
        else if (op == 0) return;
        else cout << "Opcion invalida.\n";
    }
}