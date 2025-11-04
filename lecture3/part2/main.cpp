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
            count_++;
        };
        ~Material(){
            printf("Material Destuctor!It has been read %d times\n",count_);
        };
private:
        int count_=0;
};

int main()
{
    cout << "--- 构造函数 ---" << endl;
    Material m;
    Material n;

    cout << "\n--- print函数 ---" << endl;
    m.print();

    cout << "\n--- 自动进行析构函数 ---" << endl;
    return 0;
}