#include <stdio.h>

#define N 1000000


int main(int argc, char *argv[])
{
    int m;
    int i,j;
    char op;
    int arr[N]={0};
    int head=0;
    int min = 0;
    while (scanf("%d", &m) != EOF)
    {
        memset(arr, 0, sizeof(4*N));
        for( i = 0; i < m; i++)
        {
            scanf(" %c", &op);
            if (op == 's')
            {
               scanf("%d", &arr[head++]); 
               min = arr[0];
               for(j=1; j< head; j++)
               {
                   if (min > arr[j])
                   {
                       min = arr[j];
                   }
               }
               printf("最小值：%d\n", min);
            }
            else
            {
                head = head-1;
                if (head <= 0)
                {
                    head = 0;
                }
                if (head > 0)
                {
                    min = arr[0];
                    for(j=1; j< head; j++)
                    {
                        if (min > arr[j])
                        {
                            min = arr[j];
                        }
                    }
                    printf("最小值：%d\n", min);
                }
                else
                {
                    printf("NULL\n");
                }
                
            }
            
        }
    }
    
}