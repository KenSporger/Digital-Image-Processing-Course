#include<iostream>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

// 使用map(关联数组)
void demo1()
{
    vector<string> vec = {"leo", "leo", "ken", "ken", "ken", "jack"};
    // 使用map统计单词次数
    // 与顺序容器一样，都是模板
    map<string, int> word_cnt;
    for (auto s:vec)
    {
        word_cnt[s]++;
    }
    // 打印统计结果
    // 注意s的类型是pair
    for (auto s:word_cnt)
    {
        cout << s.first << ":" << s.second << endl;
    }
}

// 使用set(集合)
void demo2()
{
    vector<string> vec = {"leo", "leo", "ken", "ken", "ken", "jack"};
    // 列表初始化
    map<string, int> word_cnt = {{"none", 2}, {"none2", 1}};   
    // 排除选项集合,列表初始化
    set<string> exclude = {"leo", "ken"};
    for (auto s:vec)
    {
        // set中没有找到s
        if (exclude.find(s) == exclude.end())
        {
            word_cnt[s]++;
        }
    }
    for (auto s:word_cnt)
    {
        cout << s.first << ":" << s.second << endl;
    }    
}


// // 使用multimap、multiset
// void demo3()
// {
//     vector<string> vec = {"leo", "leo", "ken", "ken", "ken", "jack"};
//     // 正如之前所学，容器初始化可以使用不同类型容器的迭代器
//     map<string, int> word_cnt1(vec.begin(), vec.end());//在初始化时完成统计
//     multimap<string, int> word_cnt2(vec.begin(), vec.end()); //关键字可重复，不会合并
//     cout << "map: " << word_cnt1.size() << endl;
//     cout << "multimap: " << word_cnt2.size() << endl;
//     // 同样，set也有multi版本multiset
// }

// 使用multimap、multiset
void demo3()
{
    vector<int> vec = {1, 1, 2, 2, 3, 3};
    // 正如之前所学，容器初始化可以使用不同类型容器的迭代器(前提是元素类型相同)
    // map则不能这么做，应为元素不同
    set<int> word_cnt1(vec.begin(), vec.end());//在初始化时完成统计
    multiset<int> word_cnt2(vec.begin(), vec.end()); //关键字可重复，不会合并
    cout << "set: " << word_cnt1.size() << endl;
    cout << "multiset: " << word_cnt2.size() << endl;
}



int main()
{
    // demo1();
    // demo2();
    demo3();
}