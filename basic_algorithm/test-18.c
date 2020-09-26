/*
    n=1, 1
    n=2, 2
    n=3, 3      1,1,1 1,2 2,1
    n=4, 5      1,1,1,1  1,2,1  2,1,1  1,1,2  2,2
    n=5, 8      1,1,1,1,1   1,1,1,2  1,1,2,1  1,2,1,1  2,1,1,1  2,2,1  2,1,2  1,2,2  
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
    int num=0;
    int cout=0;
    while(scanf("%d", &num) != EOF)
    {        
       cout = 0;
       printf("原值：0x%x\n", num);
        while(num)
        {
            num = num & (num-1);
            
            printf("yunsan：0x%x\n", num);
            cout ++;
        }
        printf("%d\n", cout);

    }
    return 0;
}