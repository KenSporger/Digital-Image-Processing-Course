#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>

using namespace std;


//顺序容器增删元素
int main()
{
    vector<int> v1;

    /*************insert***************/

    //任意位置前插入元素
    //相当于push_front
    v1.insert(v1.begin(), 0);

    //可以插入列表
    v1.insert(v1.cend(), {1,2,3});

    //insert返回插入元素的新位置，利用该功能，可以实现无限push_front
    auto it = v1.begin();
    for (int i = 0; i < 10; i++)
    {
        it = v1.insert(it, i);
    }


    /*************emplace***************/

    //emplace_back、emplace_front、emplace
    //emplace在插入时构造，而非拷贝
    vector<vector<int>> v2;
    //等价于
    //vector<int> v1(10, 0);
    //v2.push_back(v1);
    v2.emplace_back(10, 0);


    /*************pop***************/

    //pop没有返回值
    v1.pop_back();


    /*************erase***************/

    //删除任意位置或范围元素
    v1.erase(v1.begin(), v1.end() - 2);



    /*************访问***************/

    //访问元素
    cout << v1.at(0);
    //下标越界不会报错，但会得到错误的值
    cout << v1[10];
    //at会报错，所以更安全
    // cout << v1.at(10);


    /**************单向列表的特殊操作****************/

    forward_list<int> flist;

    //单向列表不能在尾部添加元素
    flist.push_front(0);
    //在该元素之后插入(因为下一个元素不知道上一个元素的位置)
    flist.insert_after(flist.begin(), 0);
    //删除首元素下一个元素
    flist.erase_after(flist.begin());

    



    
}