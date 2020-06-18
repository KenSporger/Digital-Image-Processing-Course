#include<algorithm>
#include<vector>
#include<iostream>
//导入bind
#include<functional>
//泛型算法第三参数谓词的实现
using namespace std;

//重载<<，用于输出
ostream& operator<<(ostream &out, vector<string> vec)
{
    out << "[";
    auto beg = vec.begin();

    for (;beg < vec.end() - 1; beg++)
    {
        out << *beg << ", ";
    }
    out << *beg;
    out << "]";
}

bool isLonger(const string &s1, const string &s2)
{
    return s1.size() > s2.size();
}

//利用普通函数作谓词
//按字符串长度排序
void demo1()
{
    vector<string> vec = {"abc", "bcde", "cde", "defgh", "efghij"};
    cout << "原序列: " << vec << endl;
    //isLonger作为二元谓词（谓词参数是严格限制的，不能出现其他参数）
    //stable_sort:相等的元素按照原有的顺序关系排序
    stable_sort(vec.begin(), vec.end(), isLonger);
    cout << "sort: " << vec << endl;

}

//使用lambda作为谓词，可以捕获外界参数
//按字符串长度排序，统计字符串长度大于给定size的个数
void demo2()
{
    //排序同demo1
    vector<string> vec = {"abc", "bcde", "cde", "defgh", "efghij"};
    cout << "原序列: " << vec << endl;
    //isLonger作为二元谓词（谓词参数是严格限制的，不能出现其他参数）
    //stable_sort:相等的元素按照原有的顺序关系排序
    stable_sort(vec.begin(), vec.end(), isLonger);
    cout << "sort: " << vec << endl;

    //find_if查找第一个长度<=size的字符串
    int size = 4;
    //返回一个迭代器，指向找到的该元素
    //只有lambda才能获取外界参数
    auto it = find_if(vec.begin(), vec.end(), [size](const string &s)->bool{
        return s.size() <= size;
    });

    cout << "found size<=4: " << *it << endl;
    //用迭代器相减获取元素个数
    cout << ">size amount: " << vec.end() - it << endl;
}  

//捕获列表
void demo3()
{
    int i = 10;
    //lambda函数在创建时，便拷贝了捕获列表参数，因此在调用时使用的值就是创建时候的值。
    auto f = [i](){return i;};
    i = 0;
    cout << "捕获列表拷贝: "<< f() << endl;
    //通过引用创建lambda，那么调用时候会去查询原值，这与拷贝不同
    auto f2 = [&i](){return i;};
    i = 10;
    cout << "捕获列表引用: " << f2() << endl;
    
}

//二元的普通函数不能作为find_if的谓词
bool findSize(const string &s, size_t size)
{
    return s.size() <= size;
}

//使用普通函数+bind实现与lambda相同效果
void demo4()
{
    //排序同demo1
    vector<string> vec = {"abc", "bcde", "cde", "defgh", "efghij"};
    cout << "原序列: " << vec << endl;
    //isLonger作为二元谓词（谓词参数是严格限制的，不能出现其他参数）
    //stable_sort:相等的元素按照原有的顺序关系排序
    stable_sort(vec.begin(), vec.end(), isLonger);
    cout << "sort: " << vec << endl;

    //find_if查找第一个长度<=size的字符串
    int size = 4;

    //不同用普通的二元函数作为find_if的一元谓词
    // auto it = find_if(vec.begin(), vec.end(), findSize);
    //对于bind函数，其参数只有占位符_1一个
    auto it = find_if(vec.begin(), vec.end(), bind(findSize, placeholders::_1, size));
    cout << "found size<=4: " << *it << endl;
    //用迭代器相减获取元素个数
    cout << ">size amount: " << vec.end() - it << endl;
}

int main()
{
    // demo1();
    // demo2();
    // demo3();
    demo4();
}