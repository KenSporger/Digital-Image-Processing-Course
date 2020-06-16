#include<algorithm>
#include<vector>
#include<iostream>
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


int main()
{
    // demo1();
    demo2();
}