#include<iostream>
//导入所有顺序容器
#include<vector>
#include<list>
#include<forward_list>
#include<deque>
#include<array>
#include<string>
#include<stack>
#include<queue>

using namespace std;


int main()
{
    //栈操作,后入先出
    stack<int> stk;
    stk.push(0);
    stk.push(1);
    stk.push(2);

    //弹出2
    cout << stk.top() << endl;
    stk.pop();
    cout << stk.top() << endl;

    //队列,先入先出
    queue<int> que;

    //注意对空的队列和栈弹出会导致数据错误
    que.pop();

    que.push(0);
    que.push(1);
    que.push(2);

    //弹出队首0
    cout << que.front() << endl;
    que.pop();
    cout << que.front() << endl;

}