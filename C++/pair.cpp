#include<iostream>
#include<algorithm>

using namespace std;

int main()
{
    // pair中的两个元素类型可以不同
    pair<string, int> p2("p2", 0);
    
    pair<int, int> p1(0, 0);
    pair<int, int> p3(1, 0);
    pair<int, int> p4(0, 1);
    pair<int, int> p5(0, 0);

    // pair会先比较第一个元素的大小
    bool flag1 = p1 < p3;
    // 如果第一个大小一样，则比较第二个
    bool flag2 = p1 < p4;
    bool flag3 = p1 == p5;
    cout << flag1 << endl;
    cout << flag2 << endl;
    cout << flag3 << endl;

    pair<int, int> item[10];
    for (int i = 0; i < 10; i++){
        item[i] = make_pair(10 - i , 10 - i);
    }

    sort(item, item + 10);
    for (int i = 0; i < 10; i++){
        cout << item[i].first << " " << item[i].second << endl;
    }
    

}