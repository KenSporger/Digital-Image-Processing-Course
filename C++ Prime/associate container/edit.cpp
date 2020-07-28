#include<iostream>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

//关联容器迭代器
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



int main()
{
    demo1();
}