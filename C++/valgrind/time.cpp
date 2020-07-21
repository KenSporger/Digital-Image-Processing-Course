#include<iostream>
#include<math.h>

using namespace std;

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