#include <iostream>
#include "recurso.h"
#include "tsp.h"

using namespace std;

int main() {
    int op;

    while (true) {
        cout << "\n========== MENU PRINCIPAL ==========\n";
        cout << "1) Problema de Distribucion de Recurso\n";
        cout << "2) Problema del Vendedor (TSP)\n";
        cout << "0) Salir\n";
        cout << "Seleccione opcion: ";
        cin >> op;

        if (op == 1) menuRecurso();
        else if (op == 2) menuTSP();
        else if (op == 0) break;
        else cout << "Opcion invalida.\n";
    }

    return 0;
}