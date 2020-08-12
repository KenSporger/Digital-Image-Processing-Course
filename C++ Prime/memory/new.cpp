#include<iostream>
#include<memory>
// g++ new.cpp -o a.out
// valgrind --leak-check=full --show-reachable=yes --trace-children=yes   ./a.out
using namespace std;

// new \ delete
void demo1()
{
    // 初始化为10
    int *pi = new int(10);
    // 初始化为0
    int *pi2 = new int();
    int *pi3 = pi2;

    delete pi;  
    // 不能释放两次
    // delete pi;
    delete pi3;
    // pi2指向的内存已经被释放
    // delete pi2;
}


int *func()
{
    int *p = new int(2);
    return p;
}

void demo2()
{
    int *p2 = func();
    int *p3 = p2;
    // 必须释放内存
    delete p2;
    // 指出p2不能再被使用
    p2 = nullptr;
    // 同时要把所有关联的指针都重置，但实际上很难找全所有关联的指针
    p3 = nullptr;
}

// // 内置指针和智能指针连用
// void demo3()
// {
//     shared_ptr<int> p1 = make_shared<int>(0);
//     // get返回一个内置普通指针
//     int *p2 = p1.get();
//     {
//         // p3与p1并没有关联，p3自己就把内存给释放了
//         shared_ptr<int> p3(p2);
//     }
//     // 错误，内存已经被释放
//     cout << *p1 << endl;
// }

// unique+reset
void demo4()
{
    // shared_ptr<int> p1();
    shared_ptr<int> p1= make_shared<int>(0);
    shared_ptr<int> p2(p1);

    // 判断指向内存的是否只有p2指针
    if (!p2.unique())
    {
        // 为p2创建副本
        p2.reset(new int(*p2));
    }

    *p2 = 1;
    cout << *p1 << endl;
    cout << p1.unique() << endl;

}

int main()
{
    // demo1();
    // demo2();
    // demo3();
    demo4();
}