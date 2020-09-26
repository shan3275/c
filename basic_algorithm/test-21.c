#include <stdio.h>
#include <stdlib.h>

typedef struct Node_t
{
    int element;
    struct Node_t * next;
}Node;

void insertOneElement(Node *head, int element)
{
    //找到尾结点
    Node *next = head;
    while (next->next)
    {
        next = next->next;
    }
    
    Node *node = malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    node->element = element;
    next->next  = node;
}

int findOneElement(Node *head, int index, int count)
{
    int i=0;
    int Rindex = count - index;
    Node *next = head->next;
    while (next)
    {
        i++;
        if ( i == Rindex +1)
        {
            return next->element;
        }
        next = next->next;
    }

    return 0;
}

void freeList(Node *head)
{
    Node *next = head;
    Node *tmp;
    while (next)
    {
        tmp = next;
        next = next->next;
        free(tmp);
        printf("释放\n");
    }
}

int main(int argc, char *argv[])
{
    int m,n;
    int element;
    int i;
    int rv;
    Node *head;

    while (scanf("%d %d", &m, &n) != EOF)
    {
        head = malloc(sizeof(Node));
        memset(head, 0, sizeof(Node));
        for (i = 0; i < m; i++)
        {
            scanf("%d", &element);
            insertOneElement(head, element);
        }
        rv = findOneElement(head, n, m);
        if (rv)
        {
            printf("%d\n", rv);
        }
        else
        {
            printf("NULL\n");
        }

        //释放
        freeList(head);

    }
    
}