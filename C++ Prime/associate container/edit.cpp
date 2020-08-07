#include<iostream>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

//关联容器迭代器是const
void demo1()
{
    map<string, int> word_cnt={{"ken", 2}};
    auto map_it = word_cnt.begin(); //map_it指向pair<const string, int>
    cout << map_it->first << ":" << map_it->second << endl;
    // 错误，关键字是const的
    // map_it->first = "a";

    set<int> word_set ={1};
    set<int>::iterator set_it = word_set.begin(); //set的迭代器是const的
    cout << *set_it << endl;
    //  错误，只读
    // *set_it = 0;
}


// 插入元素（不关心插入的位置）
void demo2()
{
    vector<int> vec= {1, 2, 3, 1, 2};
    set<int> set1;
    // 使用迭代器范围插入
    set1.insert(vec.begin(), vec.end());
    // 使用列表插入
    set1.insert({1,4});

    map<string, int> map1;
    // map添加元素必须是pair
    auto ret = map1.insert({"a", 1});
    // 返回是否插入有效（重复为false）
    cout << ret.second << endl;
    // 返回插入pair元素的计数器
    cout << ret.first->second << endl;
    map1.insert(make_pair("a", 1));
    map1.insert(pair<string, int>("a", 1));
    // value_type指的是一个pair类型
    map1.insert(map<string, int>::value_type("a", 1));
}

// 删除元素
void demo3()
{
    multimap<string, int> map1={{"a",1}, {"a",2}};
    // erase提供一个key_type参数，删除所有匹配的元素，返回删除的个数
    // multimap返回0或者大于等于1的cnt
    // map返回0或1
    auto cnt = map1.erase("a");
    cout << cnt << endl;
}


int main()
{
    // demo1();
    // demo2();
    demo3();
}