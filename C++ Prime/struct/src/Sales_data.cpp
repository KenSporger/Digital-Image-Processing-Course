#include "Sales_data.h"

using namespace std;

Sales_data &Sales_data::combine(const Sales_data &data)
{
    units_sold += data.units_sold;
    revenue += data.revenue;
    return *this;
}

//等价于double Sales_data::avg_price(const Sales_data *const this)
//成员函数内不能修改this指向的对象
double Sales_data::avg_price() const
{
    if (units_sold)
    {
        return revenue / units_sold;
    }
    else
    {
        return 0;
    }
}

istream &read(istream &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}

ostream &print(ostream &os, const Sales_data &item)
{
    os << item.bookNo << " " << item.units_sold << " "
        << item.revenue << " " << item.avg_price();

    return os;
}


//使用引用可以减少内存开销，而const防止内部修改
Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;
    sum.combine(rhs);
    return sum;
}