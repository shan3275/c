#include <stdio.h>
#include <stdlib.h>

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
    
    //链表头
    link head = (link) malloc(sizeof *head);
    head->item = rand()%1000;
    head->next = NULL;
    link x = head;
    
    for( i = 2;  i <=N; i++)
    {
        x->next = (link) malloc(sizeof *head);
        x = x->next;
        x->item = rand()%1000;
        x->next = NULL;
    }

    //打印链表
    x = head;
    printf("orgin link table: ");
    while (x != NULL)
    {
        printf("%d ", x->item);
        x = x->next;
    }
    printf("\n");

    //将最大元素移动到末尾
    //1.找到最大元素
    x = head->next;
    link max = head;
    while (x != NULL)
    {
        if (x->item > max->item)
        {
            max = x;
        }
        x = x->next;
    }
    printf("max->item:%d\n", max->item);

    //2.将最大元素从链表删除
     x = head;
     link y = NULL;
     while (x != NULL)
     {
         if (x == max)
         {

         }
         y = x;
         x = x->next;
           
     }
     

}