/*
    n=1,  1
    n=2,  2
    n=3,  3
    n=4,  5   
*/

#include <stdio.h>
#define N 71

int main(int argc, char *argv[])
{
    long long result[N]={0};
    int i;
    int num;
    result[0] = 0;
    result[1] = 1;
    result[2] = 2;
    for (i = 3; i < N; i++)
    {
        result[i] = result[i-1] + result[i-2];
    }

    while (scanf("%d", &num) != EOF)
    {
        printf("%lld\n", result[num]);
    }
    
    return 0;
}