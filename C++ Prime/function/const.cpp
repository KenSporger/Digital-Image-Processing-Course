#include<iostream>

using namespace std;


void fcn2(int &i)
{
    return;
}

//这里是顶层const，指i的内容不能被改变
void fcn(const int i)
{
    //不能改变i
    // i = 0;

    //不能用常量传值给非常量形参
    // fcn2(i);
    return;
}

// 顶层const在编译时会被忽略，因此发生重复定义的错误
/*
void fcn(int i)
{
    return;
}
*/

//这里是顶层const，i是指针，指i的指向不能改变
void fcn3(int *const i)
{
    return;
}
//顶层const在编译时会被忽略，因此发生重复定义的错误
/*
void fcn3(int *i)
{
    return;
}
*/


//这里是底层const，i是指针，i指向的对象的内容不能改变
void fcn4(const int *i)
{
    return;
}
//底层const不会报错误
void fcn4(int *i)
{
    return;
}





int main()
{
    int a = 1;
    //可以使用非常量传值给const形参
    fcn(a);
}