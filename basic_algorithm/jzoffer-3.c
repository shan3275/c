#include <stdio.h>

/* 最简单的方法，超出时间限制*/
int findRepeatNumber(int* nums, int numsSize){
    int i,j;
    for (i=0; i<numsSize; i++)
    {
        for (j=i+1; j<numsSize; j++)
        {
            if (nums[i] == nums[j])
            {
                return nums[i];
            }
        }
    }

    return 0;
}

/* 使用hash表 通过*/
int findRepeatNumber1(int* nums, int numsSize){
    #define N 100000
    int hash[N] = {0};
    int i;
    for (i=0; i<numsSize; i++)
    {
        hash[nums[i]]++;
        if (hash[nums[i]] >= 2)
        {
            return nums[i];
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int nums[7]={2,3,1,0,2,5,3};
    printf("result: %d\n", findRepeatNumber1(nums, 7));
}