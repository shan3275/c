#include <stdio.h>

#define N 51

int main(int argc, char *argv[])
{
    long long result[N]={0};
    int i,j;
    int num;
    result[0] = 0;
    result[1] = 1;
    result[2] = 2;
    for(i = 3; i < N; i++)
    {
        for (j=0; j<i; j++)
        {
            result[i] += result[j];
        }
        result[i]++;
    }

    while (scanf("%d", &num) != EOF)
    {
        printf("%llu\n", result[num]);
    }
    
    return 0;
}