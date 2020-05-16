#ifndef SALES_DATA_H
#define SALES_DATA_H

// #include<string.h>
#include<iostream>

//类一般定义在头文件中
struct Sales_data
{
    std::string bookNo;
    uint16_t units_sold = 0;
    double revenue = 0.0;
};



#endif