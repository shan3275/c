#include <stdio.h>

int calc(int num)
{
    if (num == 1 || num == 2)
    {
        return 1;
    }
    return calc(num-1)+calc(num-2);
}

#define N 71
int main(int argc, char *argv[])
{
    int num;
    int i;
    long long result[N];
    result[0] = 0;
    result[1] = 1;
    for (i=2; i<N; i++ )
    {
        result[i] = result[i-1] + result[i-2];
    }

    while(scanf("%d",&num) != EOF)
    {
        printf("%lld\n", result[num]);
    }
    return 0;
}