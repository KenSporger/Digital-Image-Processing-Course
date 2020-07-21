#include<iostream>
#include<math.h>

using namespace std;

//编译单文件指令 g++ -pg -o demo main1.cpp
//运行demo，后生成gmon.out
//分析指令 gprof demo gmon.out > report.txt 
//-p 只显示时间消耗，不显示调用关系
//-b 不显示废话（推荐）
//gprof不支持多线程
//gcc命令参数： -o 自定义输出文件名 -g 在编译的时候，产生调试信息 -On n级优化，一般为2 -l 链接库 -Wall 生成所有警告信息
//cmakelist添加方式见feature points
double func1(int i)
{
    return sqrt(i);
}

double func2(int i)
{
    return sqrt(i+1);
}


int main()
{
    double res;
    for (int i = 1; i < 100000; i++)
    {
        if (int(func1(i)) % 3 == 0)
        {
            res = func2(i);
        }
    }
    return 0;
}