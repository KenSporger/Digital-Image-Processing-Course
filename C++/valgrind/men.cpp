#include<iostream>

using namespace std;

//valgrind --leak-check=full --show-reachable=yes --trace-children= yes   ./a.out
int main()
{
    int n = 10;
    int *p = new int[n]();
    int a = p[5];
    //若不释放则会报错
    delete [] p;
    return 0;
}