#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <functional>
#include "tsp.h"

using namespace std;
vector<vector<double>> leerTSP() {
    int n;
    cout << "Ingrese N ciudades: ";
    cin >> n;

    vector<vector<double>> dist(n, vector<double>(n));
    cout << "Ingrese matriz NxN (use -1 si no hay camino), separado por espacios:\n";

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> dist[i][j];
            if (dist[i][j] < 0) dist[i][j] = 1e9;
        }

    return dist;
}

//GREEDY
pair<vector<int>, double> tspGreedy(vector<vector<double>>& dist) {
    int n = dist.size();
    vector<bool> vis(n, false);
    vector<int> ruta = {0};
    double costo = 0;

    vis[0] = true;
    int actual = 0;

    for (int k = 1; k < n; k++) {
        double mejor = 1e9;
        int next = -1;

        for (int j = 0; j < n; j++)
            if (!vis[j] && dist[actual][j] < mejor) {
                mejor = dist[actual][j];
                next = j;
            }

        vis[next] = true;
        ruta.push_back(next);
        costo += mejor;
        actual = next;
    }

    costo += dist[actual][0];
    ruta.push_back(0);

    return {ruta, costo};
}

//EXHAUSTIVA
tuple<vector<int>, double, int> tspExhaustiva(vector<vector<double>>& dist) {
    int n = dist.size();

    vector<int> perm(n - 1);
    for (int i = 1; i < n; i++) perm[i - 1] = i;

    vector<int> mejorRuta;
    double mejorCosto = 1e9;
    int contador = 0;

    do {
        vector<int> ruta = {0};
        ruta.insert(ruta.end(), perm.begin(), perm.end());
        ruta.push_back(0);

        double costo = 0;
        bool valido = true;

        for (int i = 0; i < n; i++) {
            double d = dist[ruta[i]][ruta[i + 1]];
            if (d >= 1e9) valido = false;
            costo += d;
        }

        if (valido) {
            contador++;
            if (costo < mejorCosto) {
                mejorCosto = costo;
                mejorRuta = ruta;
            }
        }

    } while (next_permutation(perm.begin(), perm.end()));

    return {mejorRuta, mejorCosto, contador};
}

//BACKTRACKING
vector<bool> visT;
vector<int> rutaAct, mejorR;
double mejorRCosto;
int contadorBT;

void btTSP(int actual, int cnt, double suma, vector<vector<double>>& dist) {
    int n = dist.size();

    if (cnt == n) {
        double total = suma + dist[actual][0];
        contadorBT++;
        if (total < mejorRCosto) {
            mejorRCosto = total;
            mejorR = rutaAct;
            mejorR.push_back(0);
        }
        return;
    }

    for (int j = 0; j < n; j++) {
        if (!visT[j] && dist[actual][j] < 1e9) {
            visT[j] = true;
            rutaAct.push_back(j);
            btTSP(j, cnt + 1, suma + dist[actual][j], dist);
            rutaAct.pop_back();
            visT[j] = false;
        }
    }
}

tuple<vector<int>, double, int> tspBacktracking(vector<vector<double>>& dist) {
    int n = dist.size();
    visT.assign(n, false);
    rutaAct.clear();
    mejorR.clear();

    mejorRCosto = 1e9;
    contadorBT = 0;

    visT[0] = true;
    rutaAct.push_back(0);

    btTSP(0, 1, 0, dist);

    return {mejorR, mejorRCosto, contadorBT};
}

//MENU
void menuTSP() {
    auto dist = leerTSP();

    while (true) {
        cout << "\n--- PROBLEMA DEL VENDEDOR (TSP) ---\n";
        cout << "1) Greedy\n";
        cout << "2) Exhaustiva\n";
        cout << "3) Backtracking\n";
        cout << "0) Volver\n";
        cout << "Seleccione: ";

        int op;
        cin >> op;

        if (op == 1) {
            auto r = tspGreedy(dist);
            cout << "Ruta: ";
            for (int x : r.first) cout << x << " ";
            cout << "\nCosto total: " << r.second << "\n";
        }
        else if (op == 2) {
            auto r = tspExhaustiva(dist);
            cout << "Mejor ruta: ";
            for (int x : get<0>(r)) cout << x << " ";
            cout << "\nCosto total: " << get<1>(r) << "\n";
            cout << "Soluciones evaluadas: " << get<2>(r) << "\n";
        }
        else if (op == 3) {
            auto r = tspBacktracking(dist);
            cout << "Mejor ruta: ";
            for (int x : get<0>(r)) cout << x << " ";
            cout << "\nCosto total: " << get<1>(r) << "\n";
            cout << "Soluciones evaluadas: " << get<2>(r) << "\n";
        }
        else if (op == 0) return;
        else cout << "Opcion invalida.\n";
    }
}
