#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    //在{}中使用多线程
    #pragma omp parallel    
    {
        cout << "parallel run!" << endl;
    }

    //for循环使用4线程
    #pragma omp parallel for num_threads(4)
    for (int i=0; i < 10; i++)
    {
        cout << i ;
    }


}