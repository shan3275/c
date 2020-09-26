#include <stdio.h>

#define N 81

int arr[N] ={0};

//投塞子
int lastRemaining(int n, int sum)
{
    int i;
    if (n == 0)
    {
        arr[sum]++;
    }
    else
    {
        for (i =1; i<=6; i++)
        {
            lastRemaining(n-1, sum+i);
        }
    }
}

void printResult(int n)
{
    int i;
    int sum=0;
    for (i=n; i<=6*n; i++)
    {
        sum = sum+ arr[i];
    }
    for (i=n; i<=6*n; i++)
    {
        printf("%d->%0.2f ", i, ((float)arr[i])/sum);
    }
    printf("\n");
}

int main()
{
    //freopen("test.txt", "r", stdin);

    unsigned int n, m;
    while(scanf("%d", &n) != EOF && n != 0)
    {
        memset(arr, 0,sizeof(int)*N);
        lastRemaining(n, 0);
        printResult(n);
    }
    return 0;
}