#include <iostream>
using namespace std;

class Material
{
    public:
        Material(){
            printf("Material Default Constructor!\n");
        };
        void print(){
            printf("This is a Material Object!\n");
        };
        ~Material(){
            printf("Material Destuctor!\n");
        };
};

int main()
{
    cout << "--- 构造函数 ---" << endl;
    Material m;

    cout << "\n--- print函数 ---" << endl;
    m.print();

    cout << "\n--- 自动进行析构函数 ---" << endl;
    return 0;
}