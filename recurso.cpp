#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "recurso.h"

using namespace std;
void leerTablaRecurso(vector<vector<double>>& tabla, int& R) {
    int N, M;
    cout << "Ingrese N (max unidades del recurso): ";
    cin >> N;
    cout << "Ingrese M (cantidad de elementos): ";
    cin >> M;

    tabla.assign(N + 1, vector<double>(M));

    cout << "Ingrese la tabla (N+1)xM, separado por espacios:\n";
    for (int i = 0; i <= N; i++)
        for (int j = 0; j < M; j++)
            cin >> tabla[i][j];

    cout << "Ingrese R (cantidad de recurso a repartir): ";
    cin >> R;
}

//GREEDY
pair<vector<int>, double> recursoGreedy(vector<vector<double>>& tabla, int R) {
    int M = tabla[0].size();
    vector<int> asign(M, 0);

    while (R > 0) {
        double mejorInc = -1e9;
        int mejorJ = -1;

        for (int j = 0; j < M; j++) {
            double inc = tabla[asign[j] + 1][j] - tabla[asign[j]][j];
            if (inc > mejorInc) {
                mejorInc = inc;
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

//EXHAUSTIVA
tuple<vector<int>, double, int> recursoExhaustiva(vector<vector<double>>& tabla, int R) {
    int N = tabla.size() - 1;
    int M = tabla[0].size();

    vector<int> mejor(M), actual(M);
    double mejorTotal = -1e9;
    int contador = 0;

    function<void(int, int)> bt = [&](int j, int restante) {
        if (j == M) {
            if (restante == 0) {
                contador++;
                double suma = 0;
                for (int i = 0; i < M; i++)
                    suma += tabla[actual[i]][i];

                if (suma > mejorTotal) {
                    mejorTotal = suma;
                    mejor = actual;
                }
            }
            return;
        }

        for (int x = 0; x <= min(restante, N); x++) {
            actual[j] = x;
            bt(j + 1, restante - x);
        }
    };

    bt(0, R);
    return {mejor, mejorTotal, contador};
}

//BACKTRACKING
tuple<vector<int>, double, int> recursoBacktracking(vector<vector<double>>& tabla, int R) {
    int N = tabla.size() - 1;
    int M = tabla[0].size();

    vector<int> mejor(M), actual(M);
    double mejorTotal = -1e9;
    int contador = 0;

    double maxValor = 0;
    for (auto& fila : tabla)
        for (double x : fila)
            maxValor = max(maxValor, x);

    function<void(int, int, double)> bt = [&](int j, int restante, double suma) {
        if (j == M) {
            if (restante == 0) {
                contador++;
                if (suma > mejorTotal) {
                    mejorTotal = suma;
                    mejor = actual;
                }
            }
            return;
        }

        double cota = suma + restante * maxValor;
        if (cota <= mejorTotal) return;

        for (int x = 0; x <= min(restante, N); x++) {
            actual[j] = x;
            bt(j + 1, restante - x, suma + tabla[x][j]);
        }
    };

    bt(0, R, 0);
    return {mejor, mejorTotal, contador};
}

//MENU
void menuRecurso() {
    vector<vector<double>> tabla;
    int R;
    leerTablaRecurso(tabla, R);

    while (true) {
        cout << "\n--- DISTRIBUCION DE RECURSO ---\n";
        cout << "1) Greedy\n";
        cout << "2) Exhaustiva\n";
        cout << "3) Backtracking\n";
        cout << "0) Volver\n";
        cout << "Seleccione: ";

        int op;
        cin >> op;

        if (op == 1) {
            auto [asig, total] = recursoGreedy(tabla, R);
            cout << "Asignacion:\n";
            for (int i = 0; i < asig.size(); i++)
                cout << "Elemento " << i << ": " << asig[i] << "\n";
            cout << "Total = " << total << "\n";
        }
        else if (op == 2) {
            auto r = recursoExhaustiva(tabla, R);
            cout << "Mejor asignacion: ";
            for (int x : get<0>(r)) cout << x << " ";
            cout << "\nTotal = " << get<1>(r) << "\n";
            cout << "Soluciones evaluadas = " << get<2>(r) << "\n";
        }
        else if (op == 3) {
            auto r = recursoBacktracking(tabla, R);
            cout << "Mejor asignacion: ";
            for (int x : get<0>(r)) cout << x << " ";
            cout << "\nTotal = " << get<1>(r) << "\n";
            cout << "Soluciones evaluadas = " << get<2>(r) << "\n";
        }
        else if (op == 0) return;
        else cout << "Opcion invalida.\n";
    }
}
