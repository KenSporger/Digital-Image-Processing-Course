#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>

using namespace std;


//顺序容器的初始化与赋值拷贝
int main()
{
    /******************初始化********************/

    //列表初始化
    deque<int> dq1={1,2,3,4};

    //初始化10个0
    list<double> list1(10, 0);

    //同类型拷贝初始化
    deque<int> dq2(dq1);
    //等价于赋值拷贝
    dq2 = dq1;

    //范围拷贝可以不同类型的容器
    deque<int> dq3(list1.begin(), list1.end());

    //array必须初始化为固定大小
    array<int, 5> ary1={1,2,3,4,5}, ary2={1,2,3,4,5};
    
    
    /******************赋值********************/

    //不同于内置数组，顺序容器都支持赋值拷贝
    ary1 = ary2;

    //assign范围赋值拷贝
    dq1.assign(list1.begin(), list1.end());
    //assign指定大小赋值
    list1.assign(10, 0);

    //array不具备assign
    //ary1.assign()

    //交换两个容器，对于array来说是真正的元素交换，因此速度与元素数量有关
    swap(ary1, ary2);
    //必须为同一容器，容器的数据类型可以不同，除了array，其他容器的交换速度都很快
    swap(dq1, dq2);


    /******************比较********************/
    //顺序容器支持比较操作
    bool flog = dq1 > dq2;


}