#include<stdio.h>
 
int cal(int num)
{
    int k=0;
    int max = 0;
    while (num)
    {
        if (num & 1)
        {
            k++;
            if (k>max)
            {
                max=k;
            }
        }
        else
        {
            k = 0;
        }
        
        num = num >>1;
    }
    return max;
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