#include <stdio.h>
#define N 100001

int input[N];



int maxSubArray(int* nums, int numsSize)
{
    int i=0,maxSum=-2147483647,currentSum=0;
    int j=0,k=0;
    while(i<numsSize){
        currentSum+=nums[i++];
        if(currentSum>maxSum) 
        {
            maxSum=currentSum;
            k = i-1;
        }
        
        if(currentSum<0) 
        {
            currentSum=0;
            j = i;
        }
    
    }
    printf("begin:%d, end:%d\n", j,k);
    return maxSum;
}


/*
arr:    -2 1 -3 4 -1 2 1 -5 4
        0  1  2 3 4  5 6  7 8
        6 3 6
        4 -1 2 1
*/
int main()
{
    //freopen("/home/lintingbin/test.txt", "r", stdin);
    int n, i, start, end, sum, start_test, end_test, sum_test;

    while (scanf("%d", &n) != EOF)
    {
        for (i = 0; i < n; i++)
            scanf("%d", &input[i]);
        
        printf("max: %d\n",  maxSubArray(input, n));   
        start = end = start_test = end_test = 0;
        sum = sum_test = input[0];
        for (i = 1; i < n; i++)
        {
            if (sum_test < 0)
            {
                start_test = end_test = i;
                sum_test = input[i];
            }
            else
            {
                sum_test += input[i];
                end_test = i;
            }
            if (sum_test > sum || (sum_test==sum && (end_test-start_test) > (end-start)))
            {
                sum = sum_test;
                start = start_test;
                end = end_test;
            }
        }
        if (n > 0)
            printf("%d %d %d\n", sum, start, end);
        else
            printf("\n");
    }



    return 0;
}