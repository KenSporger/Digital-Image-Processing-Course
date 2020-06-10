#include<iostream>

using namespace std;

//perator关键字（重载操作符）
//在编译器实现的时候，已经为我们提供了这个操作符的基本数据类型实现版本，但是现在他的操作数变成了用户定义的数据类型class，所以，需要用户自己来提供该参数版本的实现。

class Person
{
    public:

    int age;
    //默认初始化
    Person(){};
    Person(int a)
    {
        age = a;
    }
    //相等比较的重载
    //右值作为函数参数传入
    bool operator ==(const Person &p)
    {
        return age == p.age;
    }

    //拷贝重载
    //无返回值，不支持连续赋值表达式
    // void operator =(const Person &p)
    // {
    //     this->age = p.age;
    // }
    //这里返回引用，其实与不是引用没什么区别,但一般都会返回引用，以支持其他操作
    Person &operator =(const Person &p)
    {
        this->age = p.age;
        return *this;
    }

    //右增
    Person &operator ++()
    {
        this->age++;
        return *this;
    }

    //左增，需要加一个形参(类型同age)
    Person operator ++(int)
    {
        Person temp(*this);
        this->age++;
        return temp;
    }

};


int main()
{
    //默认初始化
    Person p1;
    cout << "p1.age: " << p1.age << endl;

    Person p2(10), p3(0);
    //false
    cout << "p1==p2: " << (p1 == p2) << endl;
    //相等
    cout << "p1==p3: " << (p1 == p3)<< endl;

    //三者相等
    p3 = p1 = p2;
    cout << "p1.age: " << p1.age << endl;
    cout << "p3.age: " << p3.age << endl;

    cout << "p3++.age: " << p3++.age << endl;
    cout << "++p3.age" << ++p3.age << endl;

    //只有=返回引用才能实现该混合运算
    ++(p1 = p2);
    cout << "p1.age: " << p1.age << endl;
    //未改变
    cout << "p2.age: " << p2.age << endl;



}

