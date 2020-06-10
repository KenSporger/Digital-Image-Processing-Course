#include<iostream>
#include<vector>
#include<algorithm>

bool cmp(int x, int y)
{
    return x > y;
}


using namespace std;

int main()
{
    vector<int> list1;
    for (int i = 0; i < 5; i++)
        list1.push_back(i);

    //sort仅支持升序
    sort(list1.begin(), list1.end());
    for (int i = 0; i < 5; i++)
        cout << list1[i]<< endl;

    //使用函数进行降序
    sort(list1.begin(), list1.end(), cmp);
    for (int i = 0; i < 5; i++)
        cout << list1[i]<< endl;

    //由于cmp是一次性使用的，采用lambda更好
    sort(list1.begin(), list1.end(), [](int x, int y)->bool{return x>y;} );
    for (int i = 0; i < 5; i++)
        cout << list1[i] << endl;    

    //捕获列表[]
    int total = 0;
    sort(list1.begin(), list1.end(), [&total](int x, int y)->bool{
        total += x;
        return x>y;
    } );
    cout << total << endl;

}