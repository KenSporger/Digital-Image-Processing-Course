#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>
//迭代器定义：插入迭代器、流迭代器、反向迭代器、移动迭代器
#include<iterator>
//泛型算法
#include<algorithm>

using namespace std;

//使用容器定义的迭代器进行循环遍历
void demo1()
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

//插入迭代器
void demo2()
{
    string s1 = "abcde";
    string s2;
    list<int> lst1 = {1, 2, 3, 4};
    list<int> lst2;

    //适配一个插入迭代器
    auto it = back_inserter(s2);
    //插入元素
    //等价于lst.push_back('A')
    it = 'A';
    cout << "back_insert: " << s2 << endl;
    
    //使用自身迭代器，拷贝目标要求具有足够大的容量，而lst2是空的
    // copy(s1.begin(), s1.begin(), s2.begin());
    //使用插入迭代器，就可以使用泛型算法向空容器中添加元素。
    copy(s1.cbegin(), s1.cend(), it);
    cout << "copy s2: " << s2 << endl;

    //string容器没有push_front的操作，因此也不能为其适配front_inserter迭代器
    //front_inserter拷贝后，元素顺序颠倒
    copy(lst1.cbegin(), lst1.cend(), front_inserter(lst2));
    cout << "lst2: ";
    for_each(lst2.cbegin(), lst2.cend(), [](const int x)-> void {
        cout << x;
    });
    cout << endl;

    //string的begin()支持加法，list的不行？
    //注意起始迭代器lst2.begin()不能为const
    auto it2 = inserter(lst2, lst2.begin());
    it2 = 0;
    //insert在这里连续向前部插入，只是与front_inserter不同，其插入后维持原有顺序
    copy(lst1.cbegin(), lst1.cend(), inserter(lst2, lst2.begin()));
    cout << "lst2 insert: ";
    for_each(lst2.cbegin(), lst2.cend(), [](const int x)-> void {
        cout << x;
    });
    cout << endl;
}


//反向迭代器
void demo3()
{
    string s1 = "16305";
    
    //r表示反向，rbegin指向最后一个元素
    auto it = s1.rbegin();
    cout << "rbegin: " << *it << endl;

    cout << "s1r: ";
    for (it;it != s1.rend(); it++)
    {
        cout << *it;
    }
    cout << endl;

    //使用反向迭代器进行从大到小排序
    sort(s1.rbegin(), s1.rend());
    cout << "sort: " << s1 << endl;

    string s2 = "leo,ken,john";
    //使用反向迭代器进行find,查找最后一个单词
    auto comma = find(s2.rbegin(), s2.rend(), ',');
    //错误：反向迭代器会逆序输出
    cout << "find1: " << string(s2.rbegin(), comma) << endl;
    //comma也是一个反向迭代器，因此会向前输出，故报错
    // cout << "find2: " << string(comma, s2.rbegin()) << endl;
    //正确的做法是将反向迭代器转为普通的迭代器
    cout << "find3: " << string(comma.base(), s2.end()) << endl;

}


int main()
{

    // demo1();
    // demo2();
    demo3();

}