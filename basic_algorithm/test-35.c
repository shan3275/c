#include <stdio.h>

#define N 100000

int* getLeastNumbers(int* arr, int arrSize, int k, int* returnSize)
{
    int i,j;
    int tmp;
    for ( i = 0; i < arrSize; i++)
    {
        for ( j = i+1; j < arrSize; j++)
        {
            if (arr[i] > arr[j])
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
    int *result = malloc(sizeof(int)*k);
    for ( i = 0; i < k; i++)
    {
        result[i] = arr[i];
    }
    return result;
}

int main(int argc, char *argv[])
{
    int arr[N]={0};
    int n,m;
    int i;
    int *result=NULL;
    int resultSize=0;
    while (scanf("%d %d", &n, &m) != EOF)
    {
        for (i = 0; i < n; i++)
        {
            scanf("%d", &arr[i]);
        }
        result = getLeastNumbers(arr, n, m, &resultSize);
        for (i = 0; i<m; i++)
        {
            printf("%d ", result[i]);
        }
        printf("\n");
    }
    
}