#include<iostream>
#include<vector>

using namespace std;

//可以返回引用和指针形参的引用和指针
int &func1(int *i)
{
    return *i;
} 

//不能返回局部变量的引用和指针
/*
int &func2()
{
    int i = 0;
    return i;
} 
*/

vector<int> func3()
{
    //列表初始化，相当于直接拷贝
    return {1, 2, 3};
}



int main()
{
    int a = 1;
    cout << "func1" << endl;
    //引用返回可以作为左值
    func1(&a) = 2;
    cout << a << endl;



}