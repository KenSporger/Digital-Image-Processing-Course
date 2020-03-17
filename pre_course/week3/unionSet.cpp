#include<iostream>
#include<vector>
using namespace std;

vector<int> set;
vector<int> ranks;

//集合初始化
void setInit(int n)
{
    for (int i = 0; i < n; i++)
    {
        //秩初始化为0
        ranks.push_back(0);
        //各自为一类
        set.push_back(i);
    }
}

//查找树根
int findRoot(int x)
{
    if (set[x] == x)
    {
        return x;
    }
    else
    {
        //边查找边路径压缩
        return set[x] = findRoot(set[x]);
    }
}

// 合并集合
void uniteSet(int x, int y)
{
    x = findRoot(x);
    y = findRoot(y);

    if (x == y)
    {
        return;
    }
    // 秩优化：秩高的合并秩低的，有效控制树的深度
    if (ranks[x] < ranks[y])
    {
        set[x] = y;
    }
    else
    {
        set[y] = x;
        if (ranks[x] == ranks[y])
        {
            ranks[x]++;
        }
    }
}

//判断树根是否相同
bool checkRoots(int x, int y)
{
    return findRoot(x) == findRoot(y);
}
int main()
{
    setInit(100);
    uniteSet(10,20);
    uniteSet(10,30);
    uniteSet(20,50);
    uniteSet(50,15);

    uniteSet(3, 8);
    uniteSet(16,12);
    uniteSet(3,7);
    uniteSet(21,19);
    uniteSet(16,19);
    uniteSet(16,8);
    uniteSet(15, 16);

    cout << checkRoots(10, 19);

}