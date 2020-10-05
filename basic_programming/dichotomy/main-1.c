#include <stdio.h>

// 采用递归方法
int BSearch(int d[], int target, int low, int high)
{
    int m;
    if (low <=high)
    {
        m = (low + high)/2;
        if (d[m] > target)
        {
            return BSearch(d, target, low, m-1);
        }
        else if (d[m] < target)
        {
            return BSearch(d, target, m+1, high);
        }
        else
        {
            return m;
        }
    }
    return -1;
    
}
 
int  main(int argc, char *argv[])
{
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int c = BSearch(a, 5, 0, 10);
	printf("index:%d\n", c);
}