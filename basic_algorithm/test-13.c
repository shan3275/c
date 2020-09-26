#include <stdio.h>
#include <stdlib.h>


#define N 100000
typedef struct queue_t
{
    int element[N+1];
    int num;
    int head;
    int tail;
}queue;

int main(int argc, char *argv[])
{
    int num;
    int i;
    int element;
    char buff[100];
    queue * que;
    que = malloc(sizeof(queue));
    memset(que, 0, sizeof(queue));

    if (scanf("%d", &num))
    {
        que->num = num;
        for( i=0; i< num; i++)
        {
            memset(buff, 0, 100);
            if (scanf("%s", buff))
            {
                if (buff[1] == 'U' || buff[1] == 'u')
                {
                    scanf("%d", &element);
                    que->element[que->head%num] = element;
                    que->head++;
                }
                else
                {
                    if (que->head-que->tail ==0)
                    {
                        printf("-1\n");
                    }
                    else
                    {
                        printf("%d\n", que->element[que->tail%num]);
                        que->tail++;
                    }
                }
 
            }
        }
        
    }
    
    return 0;
}