#include <iostream>
#include "asignacion.h"
#include "mochila.h"
#include "recurso.h"
#include "tsp.h"

using namespace std;

int main() {
    int op;

    while (true) {
        cout << "\n========== MENU PRINCIPAL ==========\n";
        cout << "1) Problema de Asignacion 1 a 1\n";
        cout << "2) Problema de Distribucion de Recurso\n";
        cout << "3) Problema de la Mochila\n";
        cout << "4) Problema del Vendedor\n";
        cout << "0) Salir\n";
        cout << "Seleccione opcion: ";
        cin >> op;

        switch (op) {
            case 1:
                menuAsignacion();
                break;

            case 2:
                menuRecurso();
                break;

            case 3:
                menuMochila();
                break;

            case 4:
                menuTSP();
                break;

            case 0:
                cout << "Saliendo...\n";
                return 0;

            default:
                cout << "Opcion invalida.\n";
        }
    }
}
