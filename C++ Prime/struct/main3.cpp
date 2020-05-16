#include<iostream>
#include "Sales_data.h"


//使用Sales_data构造函数
int main()
{
    Sales_data data1;
    Sales_data data2("18041124");
    Sales_data data3("18041124", 20, 10);

    
    print(std::cout, data1);
    print(std::cout, data2);
    print(std::cout, data3);

    

}