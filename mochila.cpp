#include "mochila.h"
#include <stdio.h>
#include <iostream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <functional>

//PUBLIC::::::::::::::::::::::::::::::::::::::::::::::::::::::

//constructor
mochila::mochila()
{

}

//destructor
mochila::~mochila()
{
    delete [] values;
    delete [] weights;
}

void mochila::readM()
{
    std::vector<int> pes;
    std::vector<int> val;
    bool done = false;
    while(!done)
    {
        std::cout<<"inserte el valor del elemento\n";
        int v;
       
        std::cin >> v;
     
        std::cout<<"inserte el peso del elemento\n";
        int p;
        std::cin >> p;

        val.push_back(v);
        pes.push_back(p);
        
        std::cout<<"insertar otro elemento?\n1)yes\n2)no\n";
        int yes;

        std::cin >> yes;

        if(yes == 2)
        {
            done = true;
        }
    }
    std::cout<<"cuanto es el peso maximo\n";
    std::cin>>this->bagCapacity;

    this->length = val.size();
    this->values = new int[length];
    this->weights = new int[length];

    for (int i = 0; i < length; i++) {
        this->values[i] = val[i];
        this->weights[i] = pes[i];
    }
}

//menu para elejir el tipo de busqueda
void mochila::menu()
{

    bool exit = false;
    while(!exit)
    {
        this->elements.clear();
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

//PRIVATE:::::::::::::::::::::::::::::::.:::::::::::::::::::::.

void mochila::greedy()
{
    auto start = std::chrono::high_resolution_clock::now();
    double greed[length];
    for(int i = 0; i < length; i++)
    {
        greed[i] = (double)values[i] / weights[i];
    }

    bool done = false;
    while(!done)
    {
        double max = 0;
        int idex;
        for(int i = 0; i < length; i++)
        {
            if(max<greed[i])
            {
                max = greed[i];
                idex = i;
            }
        }
        
        if(this->bagCapacity - weights[idex]>=0)
        {
            this->elements.push_back(idex);
            greed[idex] = -1;
            this->bagCapacity-=weights[idex];
        }else{
            done = true;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    int suma = 0;
    int p = 0;
    std::cout<<"segun greedy los elementos en la mochila son:";
    for(auto i : elements)
    {
        std::cout<<i<<",";
        suma += values[i];
        p += weights[i];
    }
    printf("\ncon un valor total de: %d y peso de %d\n", suma, p);
    std::cout << "Se encontro: " << elapsed.count() << " segundos\n";
    return;
}

void mochila::pureExh()
{
    std::vector<int> perm;
    int bestValue = 0;
    int bestWeight = 0;
    for(int i = 0; i< length; i++)
    {
        perm.push_back(i);
    }

    while(std::next_permutation(perm.begin(), perm.end()))
    {
        int mysum = 0;
        int myWeight = 0;
        std::vector<int> bag;
        for(auto x:perm)
        {
            if((myWeight + weights[x]) >bagCapacity)
            {
                break;
            }
            mysum += this->values[x];
            myWeight += this->weights[x];
            bag.push_back(x);
        }
        if(bestValue<mysum)
        {
            elements = bag;
            bestValue = mysum;
            bestWeight = myWeight;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout<<"segun exhaustiva pura los elementos son: ";
    for(auto i:elements)
    {
        std::cout<<i<<",";
    }
    std::cout<<". Con un valor total de "<<bestValue<< " y un peso de "<< bestWeight<<std::endl;
    std::cout << "Encontrado en " << elapsed.count() << " segundos\n";
    return;
}


void mochila::ramAndAcot() {
    int bestValue = 0;
    std::vector<int> bestSolution(length, 0);
    std::vector<int> currentSolution(length, 0);

    std::vector<int> idx(length);
    for (int i = 0; i < length; ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&](int a, int b) {
        double r1 = (double)values[a] / weights[a];
        double r2 = (double)values[b] / weights[b];
        return r1 > r2;
    });

    std::function<void(int, int, int)> branchAndBound = [&](int i, int currentWeight, int currentValue) {
        if (i == length) {
            if (currentValue > bestValue) {
                bestValue = currentValue;
                bestSolution = currentSolution;
            }
            return;
        }

        double bound = currentValue;
        int remainingWeight = bagCapacity - currentWeight;
        for (int j = i; j < length; ++j) {
            int idx_j = idx[j];
            if (weights[idx_j] <= remainingWeight) {
                bound += values[idx_j];
                remainingWeight -= weights[idx_j];
            } else {
                bound += values[idx_j] * ((double)remainingWeight / weights[idx_j]);
                break;
            }
        }

        if (bound <= bestValue) return;

        int idx_i = idx[i];
        if (currentWeight + weights[idx_i] <= bagCapacity) {
            currentSolution[idx_i] = 1;
            branchAndBound(i + 1, currentWeight + weights[idx_i], currentValue + values[idx_i]);
            currentSolution[idx_i] = 0; 
        }

        branchAndBound(i + 1, currentWeight, currentValue);
    };

    branchAndBound(0, 0, 0);

    elements.clear();
    for (int i = 0; i < length; ++i) {
        if (bestSolution[i] == 1) elements.push_back(i);
    }

    std::cout<<"segun ramificacion y acomtamiento los elementos son: ";
    int peso = 0;
    for(auto i: elements)
    {
        std::cout<<i<<", ";
        peso += weights[i];
    }
    std::cout<<"\ncon un valor total de: "<<bestValue<< " y un peso de "<<peso<<std::endl;
}

