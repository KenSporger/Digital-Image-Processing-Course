#include<iostream>
#include<math.h>
#include<stdlib.h>
using namespace std;

#define MAXN 1000000

//reference: https://www.cnblogs.com/zhengbin/p/4362788.html

// 判断输入是否为素数
// 素数则 return true 
bool checkPrimeNumber(int num)
{
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    
    //上限设置成 sqrt(num)
    int maxVal = sqrt(num);
    for (int factor = 3; factor < maxVal; factor+=2)
        if (num % factor == 0)  return false;

    return true;
}

// 打印出minVal~maxVal范围的所有素数，使用迭代法判断素数
void printPrimesByEnumerate(int minVal, int maxVal)
{

    cout << "primes between " << minVal << " to " << maxVal << endl;
    for (int num = minVal; num < maxVal; num++){

        bool isPrime = checkPrimeNumber(num);

        if (isPrime)    printf("%d ",num);
    }  
}

//用筛选法判断素数
void printPrimesByScreen(int maxVal)
{
    cout << "primes less than " << maxVal << endl;   
    //记录数字的状态，true说明不是素数
    bool isComp[MAXN];
    
    //已知2不是素数
    for (int num = 2; num < maxVal; num++){
        if (isComp[num]==false){
            printf("%d ",num);
            //凡是倍数的都排除掉
            for (int comp = num+num; comp < maxVal; comp += num)
                isComp[comp] = true;
        }
    }
}


int main()
{
    printPrimesByEnumerate(0,100);
    //帅选法要更高效
    printPrimesByScreen(100);

    return 0;
}