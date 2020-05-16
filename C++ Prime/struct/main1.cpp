#include<iostream>
#include "Sales_data.h"


//利用自定义的Sales_data数据结构进行简单的输入输出和运算
int main()
{
    Sales_data data1, data2;
    double price = 0;
    double total = 0;
    std::cin >> data1.bookNo >> data1.units_sold >> price;
    data1.revenue = data1.units_sold * price;
    std::cin >> data2.bookNo >> data2.units_sold >> price;
    data2.revenue = data2.units_sold * price;   

    if (data1.bookNo == data2.bookNo)
    {
        total = data1.revenue + data2.revenue;
        std::cout << "the total price is " << total << std::endl;
    }
    else
    {
        std::cerr << "the bookNo is not same!" << std::endl;
    }
    
    return 0;
}