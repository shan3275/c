#include <stdio.h>

#define N 100000

int calc(int *arr, int len)
{
    int result = -1;
    int hash[N+1] = {0};
    int i;
    for( i = 0; i < len; i++)
    {
        hash[arr[i]]++;
    }

    for (i = 0; i <N+1; i++)
    {
        if (hash[i] > (len/2))
        {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    int arr[N]={0};
    int n;
    int i;
    while (scanf("%d", &n) != EOF)
    {
        for (i = 0; i < n; i++)
        {
            scanf("%d", &arr[i]);
        }
        printf("%d\n", calc(arr, n));
    }
    
}