#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>

using namespace std;

//顺序容器的迭代器
int main()
{
    //列表初始化
    //双向链表
    list<int> list1={1,2,3,4};

    list<int>::iterator it1 = list1.begin();
    //等价于
    // auto it1 = list1.begin();

    //当不需要写访问时，使用const
    list<int>::const_iterator it2 = list1.cbegin(); 

    auto it3 = list1.end();
    auto it4 = list1.cend();

    //反向迭代器+const
    auto it5 = list1.crbegin();

    // 循环中增减容器可能会使迭代器失效
    // 建议不要暂存end()
    // while(it2 != it4)
    // {
    //     list1.push_back(0);
    //     cout << *it2;
    //     ++it2;
    // }
    
    while(it5 != list1.crend())
    {
        cout << *it5;
        //反向迭代器仍然使用自增
        ++it5;
    }

    //单向列表
    forward_list<int> flist1={5,6,7,8};

    auto it6 = flist1.begin();
    auto it7 = flist1.end();
    //只支持单向迭代，不支持反向迭代，故没有rbegin、rend
    // auto it6 = flist1.rbegin();
    // auto it6 = flist1.rend();

    while (it6 != it7)
    {
        flist1.push_front(0);
        cout << *it6;
        ++it6;
    }
    
    









    
}