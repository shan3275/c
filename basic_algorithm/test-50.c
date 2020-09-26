#include<stdio.h>
 
int cal(int num)
{
    int i;
    int tmp;
    int tmp1;
    int sum;
    int k=0;
    for (i=0; i<= num; i++)
    {   tmp = 10;
        tmp1 = i;
        sum = i*i;
        while(tmp1/10)
        {
            tmp*=10;
            tmp1/=10;
        }
        if (sum%tmp == i)
        {
            k++;
        }
    }
    return k;
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