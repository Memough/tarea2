#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "tsp.h"

using namespace std;

vector<int> tspGreedy(vector<vector<double>>& dist, double& costo) {
    int n = dist.size();
    vector<bool> vis(n, false);
    vector<int> ruta;
    costo = 0;

    int actual = 0;
    ruta.push_back(0);
    vis[0] = true;

    for (int k = 1; k < n; k++) {
        double mejor = 1e9;
        int mejorJ = -1;
        for (int j = 0; j < n; j++) {
            if (!vis[j] && dist[actual][j] < mejor) {
                mejor = dist[actual][j];
                mejorJ = j;
            }
        }
        vis[mejorJ] = true;
        ruta.push_back(mejorJ);
        costo += mejor;
        actual = mejorJ;
    }

    costo += dist[actual][0];
    ruta.push_back(0);
    return ruta;
}

void menuTSP() {
    int n;
    cout << "Ingrese N ciudades: ";
    cin >> n;

    vector<vector<double>> dist(n, vector<double>(n));
    cout << "Ingrese matriz NxN:\n";

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> dist[i][j];

    int op;
    while (true) {
        cout << "\n--- Problema del Vendedor ---\n";
        cout << "1) Greedy\n";
        cout << "0) Volver\n";
        cin >> op;

        if (op == 1) {
            double costo;
            auto ruta = tspGreedy(dist, costo);
            cout << "Ruta: ";
            for (int x : ruta) cout << x << " ";
            cout << "\nCosto total: " << costo << "\n";
        }
        else if (op == 0) return;
        else cout << "Opcion invalida.\n";
    }
}