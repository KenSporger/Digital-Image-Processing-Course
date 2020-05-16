#include<iostream>
#include<vector>

using namespace std;

//vector二分查找
int main()
{
    vector<int> vec;
    int target = 10;
    int res = -1;

    cin >> target;

    for (int i = 0; i < 100; i++)
    {
        vec.push_back(i);
    }

    //不关心迭代器的类型
    auto beg = vec.begin();
    auto en = vec.end();
    //取中心
    auto mid = beg + (en - beg) / 2;

    //en指向尾元素的下一个元素（不存在），beg指向第一个数
    //beg == en时表示没有剩余元素
    while (beg < en)
    {
        if (*mid > target)
        {
            en = mid;
        }
        else if (*mid < target)
        {
            beg = mid + 1;
        }
        else
        {
            res = *mid;
            break;
        }

        mid = beg + (en - beg) / 2;
        
    }

    cout << res << endl;

    return 0;
    
}