#include<iostream>
#include<vector>

using namespace std;

typedef bool Func(int a, int b);

//inline 申明内联函数，相当于在调用时直接展开
inline bool func1(int a, int b)
{
    return a > b;
}

//使用typedef，简化函数形参的声明（隐式指针）
int func2(Func f)
{
    int a = 1;
    int b = 0;
    return f(a, b);
}
//和显式使用指针等价
int func3(Func *f)
{
    int a = 1;
    int b = 0;
    return f(a, b);
}



int main()
{
    cout << "func1" << endl;
    int a = 0;
    int b = 1;
    //展开时并不会受外界干扰
    int c = 2 + func1(a, b);
    cout << c << endl;

    cout << "funcp" << endl;
    //函数指针
    bool (*funcp1)(int a, int b) = func1;
    //等价于
    bool (*funcp2)(int a, int b) = &func1;
    //等价于
    Func *funcp3 = func1;
    cout << funcp1(a, b) << endl;

    cout << "func2" << endl;
    cout << func2(func1) << endl;

    cout << "func3" << endl;
    cout << func3(func1) << endl;

    

}