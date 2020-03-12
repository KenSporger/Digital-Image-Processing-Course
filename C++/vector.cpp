#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void printVector(vector<int> obj, string mes)
{
    cout << "-----------" << mes << "-----------" << endl;
    // 获取vector的size,注意如果是二维vector获取的size是第一维度大小
    cout << "size:" << obj.size() << endl << "vector:";
    // 通过已知大小来迭代访问
    for (int i; i < obj.size(); i++){
        printf("%d ", obj[i]);
    }
    cout << endl;
}

int main()
{
    vector<int> obj;

    // 在vector最后加入元素
    for (int i; i < 5; i++){
        obj.push_back(i);
    }
    printVector(obj, "initial");

    // 删除最后的元素
    obj.pop_back();
    printVector(obj, "pop");

    // reverse 从大到小排列
    // begin:开始指针
    // end:结束指针
    reverse(obj.begin(), obj.end());
    printVector(obj, "reverse");

    // sort 从小到大排列
    sort(obj.begin(), obj.end());
    printVector(obj, "sort");


    int rows = 2;
    // 定义二维vector
    vector<vector<int> > obj2(rows, obj);
    
    for (int i = 0; i < rows; i++){
        printVector(obj2[i], "row:" + to_string(i) );
    }

    // 迭代器访问元素
    vector<int>::iterator vec = obj.begin();
    while (vec != obj.end())
    {
        cout << *vec;
        vec++;
    }
    
}