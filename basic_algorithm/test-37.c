#include <stdio.h>

#define N 100000

/*
    arr :  16,7,3,20,17,8 索引从1开始
    size： 6
    先建堆
                     16
            7                    3
        20      17         8

*/

void heap_adjust(int *arr, int i, int len)
{
    int lchild, rchild,max,tmp;
    if (i <= len )
    {
        max = i;
        lchild = 2*i;
        rchild = 2*i +1;
        if ( lchild <= len && arr[lchild] > arr[max])
        {
            max = lchild;
        }
        if ( rchild <= len && arr[rchild] > arr[max])
        {
            max = rchild;
        } 
        if (max != i)
        {
            tmp = arr[max];
            arr[max] = arr[i];
            arr[i]   = tmp;
            heap_adjust(arr, max, len);
        }       
    }
}

void heap_sort(int *arr, int size)
{
    int i,j,tmp;
    //先得到初始堆
    for(i=size/2; i>=1; i--)
    {
        heap_adjust(arr, i, size);
        for(j = 1; j<=size; j++)
        {
            printf("%d ", arr[j]);
        }   
        printf("\n");
    }

    for (i=size; i>1; i--)
    {
        tmp = arr[1];
        arr[1] = arr[i];
        arr[i] = tmp;
        heap_adjust(arr, 1, i-1);

        for(j = 1; j<=size; j++)
        {
            printf("%d ", arr[j]);
        }   
        printf("\n");
    }
}

int* getLeastNumbers(int* arr, int arrSize, int k, int* returnSize)
{
    int i,j;
    int tmp;

    heap_sort(arr, arrSize);
    int *result = malloc(sizeof(int)*k);
    for ( i = 0; i < k; i++)
    {
        result[i] = arr[i+1];
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
        for (i = 1; i <= n; i++)
        {
            scanf("%d", &arr[i]);
        }
        //arr索引从1开始
        //heap_sort(arr, n);
        
        result = getLeastNumbers(arr, n, m, &resultSize);
        for (i = 0; i<m; i++)
        {
            printf("%d ", result[i]);
        }
        printf("\n");
        
    }
    
}