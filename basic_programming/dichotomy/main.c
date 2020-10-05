#include <stdio.h>
 
int BSearch(int d[], int target, int low, int high)
{
    int m;
    while (low <=high)
    {
        m = (low + high)/2;
        if (d[m] > target)
        {
            high = m-1;
        }
        else if (d[m] < target)
        {
            low = m + 1;
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