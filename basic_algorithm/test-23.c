#include <stdio.h>
#include <stdlib.h>

typedef struct Node_t
{
    int element;
    struct Node_t * next;
}Node;

void addOneElement(Node *head, int element)
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


void insertOneElement(Node *head, int element)
{
    Node *curl = head;
    Node *tmp;
    Node *node;
    int flag = 0;
    while (curl->next)
    {
        tmp = curl->next;
        if (tmp->element >= element)
        {
            node =  malloc(sizeof(Node));
            memset(node, 0, sizeof(Node));
            node->element = element;
            node->next    = tmp;
            curl ->next   = node;
            flag = 1;
            break; 
        }
        curl= curl->next;
    }
    if (flag == 0)
    {
        addOneElement(head, element);
    }
}

void revertList(Node *head)
{
    Node *curl = head->next;
    head->next = NULL;
    Node *tmp=NULL;
    while (curl)
    {
        tmp = curl;
        curl = curl->next;
        tmp->next = head->next;
        head->next = tmp;
    }
    
}

void printList(Node *head)
{
    Node *curl = head->next;
    while (curl)
    {
        printf("%d ", curl->element);
        curl= curl->next;
    }
    printf("\n");
    
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
    Node *curl = head;
    Node *tmp;
    while (curl)
    {
        tmp = curl;
        curl = curl->next;
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
            addOneElement(head, element);
        }

        for(i = 0; i < n; i++)
        {
            scanf("%d", &element);
            insertOneElement(head, element);
        }

        //revertList(head);
        printList(head);
        //释放
        freeList(head);

    }
    
}