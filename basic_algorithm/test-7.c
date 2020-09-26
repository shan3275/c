#include <stdio.h>
#include <string.h>
#define N 1000

typedef struct arr_t
{
    int num;
    struct arr_t *next;
    struct arr_t *pre;    
}arr;

void insert_one_data(arr *head, int num)
{
    arr * last = NULL;
    arr * next = head;
    while (next->next != NULL)
    {
        next = next->next;
    }

    last = malloc(sizeof(arr));
    memset(last, 0, sizeof(arr));
    last->num = num;
    last->pre = next;
    next->next = last;
}

void print_list(arr *head)
{
    arr * next = head;
    while (next->next != NULL)
    {
        next = next->next;
    }

    arr *pre = next;
    while (pre->pre != NULL)
    {
        printf("%d\n", pre->num);
        pre= pre->pre;
    }
}


int main(int argc, char *argv[])
{
    arr * head = NULL;
    head = malloc(sizeof(arr));
    memset(head, 0, sizeof(arr));
    int num;
    while (scanf("%d", &num) != EOF)
    {
        
        if (num != -1)
        {
            insert_one_data(head, num);
        }
        else
        {
             print_list(head);
             return 0;
        }
        
    }
    
    return 0;
}