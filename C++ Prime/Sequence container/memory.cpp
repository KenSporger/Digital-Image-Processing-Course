#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>

using namespace std;

//内存管理
int main()
{
    vector<int>  v;

    //当前元素个数
    cout << "size: " << v.size() << endl;
    //当前能容纳的最大元素个数
    cout << "size: " << v.capacity() << endl;

    //只有当前容量不足时才会扩展，一次扩展会留下翻倍的余量
    for (int i = 0; i < 24; i++)
    {
        v.push_back(i);
    }

    //当前元素个数=24
    cout << "size: " << v.size() << endl;
    //翻倍扩展空间，32
    cout << "size: " << v.capacity() << endl;

    //请求收回空余空间,但并不保证一定归还
    v.shrink_to_fit();
    //成功收回：24
    cout << "size: " << v.capacity() << endl;
    
    //手动设置空间
    v.reserve(50);
    cout << "size: " << v.capacity() << endl;

    //增加元素至51个，刚好超出空间
    for (int i = 0; i < 27; i++)
    {
        v.push_back(i);
    }

    
    cout << "size: " << v.size() << endl;
    //空间变为100，说明就是翻倍扩展
    cout << "size: " << v.capacity() << endl;


    

}