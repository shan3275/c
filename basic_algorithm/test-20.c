#include <stdio.h>

#define N 100

int main(int argc, char *argv[])
{
    int arr[N]={0};
    int result[N]={0};
    int num;
    int i,j,tmp,k;
    while (scanf("%d", &num) != EOF)
    {
        for (i = 0; i < num; i++)
        {
            scanf("%d", &arr[i]);
        }

           for(k=0; k < num; k++)
           {
               printf("㡳%d ", arr[k]);
           }
           printf("\n");

        for (i =j= 0; i < num; i++)
        {
            if (arr[i]%2)
            {
                result[j++] = arr[i];
            }
           
           for(k=0; k < num; k++)
           {
               printf("%d ", result[k]);
           }
           printf("\n");
        }

        for (i =0; i < num; i++)
        {
            if (arr[i]%2 == 0)
            {
                result[j++] = arr[i];
            }
           
           for(k=0; k < num; k++)
           {
               printf("%d ", result[k]);
           }
           printf("\n");
        }


    }
    
    return 0;
}