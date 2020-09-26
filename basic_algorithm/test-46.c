#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcd(int a, int b)
{
    while(a!=b)
    {
        if(a>b)
        a = a - b;
        if(b>a)
        b = b - a;
    }
    return a;
}

int main(int argc, char *argv[])
{
    int a,b;
    while (scanf("%d %d", &a, &b) != EOF)
    {
        printf("最大公约数：%d\n", gcd(a,b));
        printf("最小公倍数：%d\n", a*b/gcd(a,b));
    }
}