#include <stdio.h>

#define N 1000


void findDulpNum(int *arr, int num, int *result, int *resnum)
{
    int i;
    int tmp;
    int hash[N] = {0};
    int count=0;
    for (i=0; i < num; i++)
    {
        tmp = arr[i] % num;
        printf("i=%d, tmp=%d\n", i, tmp);
        hash[tmp]++;
    }

    for (i=0; i < num; i++)
    {
        if (hash[i]>1)
        {
            result[count++] = i;
        }
    }
    *resnum = count;
}

int main(int argc, char *argv[])
{
    int n;
    int i;
    int arr[N]={0};
    int result[N]={0};
    int resnum= 0;
    while (scanf("%d", &n) != EOF)
    {
        for(i = 0; i < n; i++)
        {
            scanf("%d", &arr[i]);
        }
        for(i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");

        findDulpNum(arr, n, result, &resnum);
        for (i = 0; i < resnum; i++)
        {
            printf("%d ", result[i]);
        }
        printf("\n");
    }
    
    return 0;
}