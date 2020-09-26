#include <stdio.h>
typedef struct node* link;
struct node 
{
    int item;
    link next;
};

int main(int argc, char *argv[])
{
    int i;
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    link t = (link) malloc(sizeof *t);
    t->item = 1;
    t->next = t;
    link x = t;
    
    for( i = 2;  i <=N; i++)
    {
        x->next = (link) malloc(sizeof *t);
        x = x->next;
        x->item = i;
        x->next = t;
    }

    while (x != x->next)
    {
        for (i=1; i<M; i++)
        {
            x = x->next;
        }
        x->next = x->next->next;
    }
    printf("%d\n", x->item);
}