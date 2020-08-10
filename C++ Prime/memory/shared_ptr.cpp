
#include<iostream>
#include<memory>
#include<vector>

using namespace std;


// 初始化分配
void demo1()
{
    // make_shared分配内存,make_shared初始化类似于emplace
    shared_ptr<string> p1 = make_shared<string>("hello");
    shared_ptr<string> p2 = make_shared<string>(10, 'h');
    // 解引用
    cout << *p2 << endl;

    // 无参数，初始化为0
    auto p3 = make_shared<int>();
    // 函数结束，所有shared_ptr都会被释放
}

// 指针拷贝
shared_ptr<int> demo2()
{
    auto p1 = make_shared<int>(1);
    auto p2 = make_shared<int>(2);
    // 拷贝，p3,p1都指向1，递增p1指向的对象的引用计数
    auto p3(p1);
    // p2也指向1，递减原p2指向对象的引用计数
    // 没有指针指向2,自动释放2的内存空间
    p2 = p1;
    // 在返回时会创建拷贝，这个拷贝不会被释放
    return p1;
}



class ptr_vector
{
    public:
        typedef vector<int>::size_type size_type;
        // 初始化空的vector
        ptr_vector();
        // 列表初始化
        ptr_vector(initializer_list<int> il);

        size_type size() const {return data->size();}
        bool empty() const {return data->empty();}

        void push_back(int i) {data->push_back(i);}
        void pop_back();
        int at(int index) {return data->at(index);}

    private:
        shared_ptr<vector<int>> data;
        // 下标检查
        void check(size_type i, const string &msg) const;
};

ptr_vector::ptr_vector():
    data(make_shared<vector<int>>()){}

ptr_vector::ptr_vector(initializer_list<int> il):
    data(make_shared<vector<int>>(il)){}

void ptr_vector::pop_back()
{
    check(0, "index error!");
    data->pop_back();
}

void ptr_vector::check(size_type i, const string &msg) const 
{
    if (i >= data->size())
        throw out_of_range(msg);
}


void demo3()
{
    vector<int> v1={1,2,3};
    {
        vector<int> v2 = {3,4,5};
        v1 = v2;
        // v2 除了作用域被销毁
    }
    ptr_vector obj1({1});
    {
        ptr_vector obj2({4,5,6});
        obj1 = obj2;
        // obj2将被销毁，但是其指向的内存空间人保留
    }
    cout << obj1.at(0) << endl;

}



int main()
{
    // demo1();
    // auto p = demo2();
    demo3();
}