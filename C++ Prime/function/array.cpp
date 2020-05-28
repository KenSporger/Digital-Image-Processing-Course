#include<iostream>

using namespace std;


//fcn1与fcn2等价，传入的数组都是指针
void fcn1(const int *p)
{
    cout << p[1];
}
void fcn2(const int p[])
{
    cout << p[0];
}

//越界保护：
//法一：传入首尾指针，end是尾元素的下一个元素
//法二：传数组长度
void fcn3(const int *beg, const int *end, const size_t len)
{
    cout << len << endl;
    while (beg != end)
    {
        cout << *beg++ ;
    }
}
    
//数组引用,必须指定数组长度
void fcn4(const int (&p)[5])
{
    for (auto i : p)
    {
        cout << i;
    }
}

//二维数组
// int *p[5]：含指针的数组，长度为5
// int (*p)[5]：指向数组的指针
void fcn5(int (*p)[5], int *q[5])
{
    //以下两者等价
    cout << p[0][1];
    cout << *(q[0]+1);
}

//用于数量未知，类型相同的实参
//initializer_list类似于vector，但是其值必须为常量
void fcn6(initializer_list<int> li)
{
    for (auto i : li)
    {
        cout << i;
    }
}


//argv从右向左，是数组，元素为char指针
int main(int argc, char *argv[])
{
    //输入的字符串个数
    cout << argc;
    //argv[0]为程序名
    //argv[1]指向第1个字符串参数
    cout << argv[1] << endl;

    int a[5] = {0, 1, 2};

    //传入的时指针
    cout << "func1" << endl;
    fcn1(&a[1]);
    cout << endl <<"func2" << endl;
    fcn2(&a[1]);
    cout << endl << "func3" << endl;
    //end决定的长度为5，手动传入的长度为3
    fcn3(begin(a), end(a), 3);
    cout << endl << "func4" << endl;
    fcn4(a);
    cout << endl << "func5" << endl;

    int b[1][5]= {{0, 1, 2}};
    int *c[5]={b[0]};

    fcn5(b, c);
    
    cout << endl << "func6" << endl;
    fcn6({1,2,3,4,5});

    return 0;
}