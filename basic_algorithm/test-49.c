#include<stdio.h>
 
int main()
{
    int getNum=0;
    int i=0,j=0,sum=0;
    while(scanf("%d",&getNum)!=EOF)
    {
        j++;
        if(getNum>0)
        {
            sum+=getNum;
            i++;
        }
    }
    printf("%d\n",j-i);
    if(i>0)
        printf("%.1f\n",(float)sum/(float)i);
    else
        printf("0.0\n");
    return 0;
}