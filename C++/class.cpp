#include<iostream>

using namespace std;

class myClass
{
    public:
        int x=0;
        void func(){
            x++;
        }
};

int main()
{
    myClass obj;
    obj.x = 10;
    // 类方法可以改变类中的变量
    obj.func();
    cout << obj.x;
}

