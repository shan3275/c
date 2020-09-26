#include<stdio.h>
 
int cal(int num)
{
    int k=0,j;
    int sum = 0;
    int count=0;
    for(k=2; k<=num; k++)
    {
        sum = 1;
        for ( j=2; j<k; j++)
        {
            if (k%j == 0)
            {
                sum += j;
            }
        }
        if (sum == k)
        {
            printf("符合：%d\n",k);
            count ++;
        }
    }

    return count;
}

int main()
{
    int getNum=0;
    int sum=0;
    while(scanf("%d",&getNum)!=EOF)
    {
        printf("%d\n", cal(getNum));
    }

    return 0;
}