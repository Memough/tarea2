#include <vector>

class unoauno{
    public:
        unoauno();
        ~unoauno();
        void readM();
        void menu();
    private:
        int **myMatriz;
        int elements;
        int tasks;
        std::vector<int> respuesta;

        
        void greedy();//greedy
        void pureExh();//exhaustiva pura
        void ramAndAcot();//ramificad y acotada

        bool isIn(int x, std::vector<int> vec);
};
