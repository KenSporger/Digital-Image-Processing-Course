#include<iostream>

using namespace std;

// 函数返回pair的三种构造方法
pair<string, int> demo1(int mode=0)
{
    switch (mode)
    {
    case 0:
        return {"ken", 11};
        break;
    case 1:
        return make_pair("ken", 11);
    default:
        return pair<string, int>("ken", 11);
        break;
    }
    return {"ken", 11};
}

int main()
{
    // 列表初始化
    pair<string, int> mypair{"ken", 11};
    demo1();

}