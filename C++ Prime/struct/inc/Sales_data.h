#ifndef SALES_DATA_H
#define SALES_DATA_H

// #include<string.h>
#include<iostream>

//类一般定义在头文件中
struct Sales_data
{
    //默认构造函数,如果没有则必须显示初始化
    Sales_data() = default;
    //显式初始化bookNo
    Sales_data(const std::string &s) : bookNo(s){}
    //显示初始化所有
    Sales_data(const std::string &s, uint16_t n, double p) :
                    bookNo(s), units_sold(n), revenue(p * n){}
    //成员函数1，const
    std::string isbn() const {return bookNo;}
    //成员函数2，引用
    Sales_data &combine(const Sales_data &data);
    //struct成员默认是public的
    // private:
        std::string bookNo = "none";
        uint16_t units_sold = 0;
        double revenue = 0.0;
        //成员函数3
        double avg_price() const;

    //将非成员函数作为友元函数，从而使其可以访问私有成员
    friend std::istream &read(std::istream &is, Sales_data &item);
    friend std::ostream &print(std::ostream &os, const Sales_data &item);
    friend Sales_data add(const Sales_data &lhs, const Sales_data &rhs);
};

std::istream &read(std::istream &is, Sales_data &item);
std::ostream &print(std::ostream &os, const Sales_data &item);
Sales_data add(const Sales_data &lhs, const Sales_data &rhs);

#endif