#include<iostream>
#include "Sales_data.h"


//使用Sales_data类成员函数和非成员函数
int main()
{
    Sales_data data1, data2, data3;

    read(read(std::cin, data1) ,data2);

    print(std::cout, data1);
    print(std::cout, data2);

    data3 = add(data1, data2);

    print(std::cout, data3);

    

}