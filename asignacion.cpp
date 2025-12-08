#include "asignacion.h"
#include <stdio.h>
#include <iostream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <functional>

//PUBLIC::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//constructor
unoauno::unoauno()
{

}

//destructor
unoauno::~unoauno()
{
    for(int i = 0; i<elements;i++)
    {
        delete[] myMatriz [i];
    }
    delete [] myMatriz;
}

void unoauno::readM()
{
    std::cout<<"ingrese cuantas tareas y elementos son (solo un numero)\n";
    int x;
    std::cin>>x;
    this->elements = x;
    this->tasks = x;
    myMatriz = new int*[elements];
    for(int i = 0; i< elements;i++)
    {
        myMatriz[i] = new int[tasks];
        for(int j = 0; j<tasks; j++)
        {
            std::cout<<"ingrese el valor de " <<i<< " "<<j<<std::endl;
            std::cin>>myMatriz[i][j];
        }
    }
}

//el menu para elejir el tipo de busqueda
void unoauno::menu()
{
    bool exit = false;
    while(!exit)
    {
        this->respuesta.clear();
        int election;
        std::cout<<":::::::::::::ELIJE EL TIPO DE BUSQUEDA:::::::::::::\n 1)Greedy\n 2)Exhaustiva Pura\n 3)Exhaustiva con Ramificación y Acotamiento\n 4)Regresar\n";

        std::cin.exceptions(std::ios::failbit | std::ios::badbit);

        try {
            std::cin >> election;
        }
        catch (const std::ios_base::failure& e)
        {
            std::cout << "ERROR\nOPCION INVALIDA. INGRESE SOLO LOS NUMEROS CORRESPONDIENTES A LAS OPCIONES MOSTRADAS EN EL MENU\n";

            std::cin.exceptions(std::ios::goodbit);

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cin.exceptions(std::ios::failbit | std::ios::badbit);

            continue;
        }

        switch(election)
        {
            case 1://greedy
                greedy();
                break;
            case 2://pura
                pureExh();
                break;
            case 3://ramificada
                ramAndAcot();
                break;
            case 4://salida
                exit = true;
                break;
            default:
                std::cout << "ERROR\nOPCION INVALIDA. INGRESE SOLO LOS NUMEROS CORRESPONDIENTES A LAS OPCIONES MOSTRADAS EN EL MENU\n";
                break;
        }
    }

}

//PRIVATE::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//hace la busqueda greedy
void unoauno::greedy()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> greed;

    for(int i = 0 ; i < this->elements; i++)
    {
        int max = 0;
        int idex;
        for(int j = 0; j < this->tasks; j++)
        {
            if(max<myMatriz[i][j])
            {
                if(!isIn(j,greed))
                {
                    max = myMatriz[i][j];
                    idex = j;
                }
                
            }
        }
        greed.push_back(idex);
        respuesta.push_back(i);
        respuesta.push_back(idex);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    int suma = 0;
    std::cout<<"Según greedy la respuesta es:";
    for(int i = 0; i < respuesta.size(); i+=2)
    {
        std::cout<<"el elemento: "<<respuesta[i]<<" es asignado al elemento: "<<respuesta[i+1]<<", ";
        suma+=myMatriz[respuesta[i]][respuesta[i+1]];
    }
    printf(" con una suma total de: %d\n", suma);
    std::cout << "Encontrado en " << elapsed.count() << " segundos\n";
    return;
}

//checks for an element inside a vector
bool unoauno::isIn(int x, std::vector<int> vec)
{
    for(auto i : vec)
    {
        if(i==x)
        {
            return true;
        }
    }
    return false;
}

//hace la busqueda exhaustiva
void unoauno::pureExh()
{
    auto start = std::chrono::high_resolution_clock::now();
    int bestResp = 0;
    std::vector<int> perm;
    for(int i = 0; i< elements; i++)
    {
        perm.push_back(i);
    }

    while(std::next_permutation(perm.begin(), perm.end()))
    {
        int mysum = 0;
        int idex = 0;
        for(auto x:perm)
        {
            mysum += myMatriz[idex][x];
            idex++;
        }
        if(mysum>bestResp)
        {
            respuesta = perm;
            bestResp = mysum;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout<<"segun exhaustiva pura la respuesta es: ";
    int idex = 0;
    for(auto i:respuesta)
    {
        std::cout<<"el elemento "<< idex<< " asignado al elemento "<<i<<", ";
        idex++;
    }
    std::cout<<". Con el valor maximo de "<<bestResp<<std::endl;
    std::cout << "Encontrado en " << elapsed.count() << " segundos\n";
    return;
}

//Búsqueda Exhaustiva con Ramificación y Acotamiento 
void unoauno::ramAndAcot() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<bool> assignedTask(tasks, false);
    std::vector<int> currentAssignment(elements, -1);
    int currentMaxCost = std::numeric_limits<int>::min();

    auto calcUpperBound = [&](int row, int cost_so_far) -> int {
        int bound = cost_so_far;
        for (int r = row; r < elements; ++r) {
            int maxVal = std::numeric_limits<int>::min();
            for (int c = 0; c < tasks; ++c) {
                if (!assignedTask[c] && myMatriz[r][c] > maxVal) {
                    maxVal = myMatriz[r][c];
                }
            }
            bound += (maxVal != std::numeric_limits<int>::min()) ? maxVal : 0;
        }
        return bound;
    };

    std::function<void(int, int)> branchAndBound = [&](int row, int cost_so_far) {
        if (row == elements) {
            if (cost_so_far > currentMaxCost) {
                currentMaxCost = cost_so_far;
                respuesta = currentAssignment;
            }
            return;
        }

        for (int col = 0; col < tasks; ++col) {
            if (!assignedTask[col]) {
                int tempCost = cost_so_far + myMatriz[row][col];

                assignedTask[col] = true;
                currentAssignment[row] = col;
                int upperBound = calcUpperBound(row + 1, tempCost);
                if (upperBound > currentMaxCost) { 
                    branchAndBound(row + 1, tempCost);
                }
                assignedTask[col] = false;
                currentAssignment[row] = -1;
            }
        }
    };

    branchAndBound(0, 0);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "segun ramificacion y acotamiento la respuesta es\n";
    for (int i = 0; i < elements; ++i) {
        std::cout << "Elemento " << i << " asignado a " << respuesta[i];
    }
    std::cout << " con el costo maximo: " << currentMaxCost << std::endl;
        std::cout << "Encontrado en " << elapsed.count() << " segundos\n";
}

