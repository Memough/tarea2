#include <vector>

class mochila
{
    public:
        mochila();
        ~mochila();
        void readM();
        void menu();
    private:
        int bagCapacity;
        int *values;
        int *weights;
        int length;
        std::vector<int> elements;

        
        void greedy();//greedy
        void pureExh();//exhaustiva pura
        void ramAndAcot();//ramificad y acotada
};
