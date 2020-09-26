/*
            2
        1        3  

*/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *lchild;
    struct node *rchild;
};

#define N 1000
struct queue
{
    struct node *arr[N];
    int num;
    int head;
    int tail;
};

struct queue *que;
void quePush(struct node *root)
{
    que->arr[que->head++] = root;
}

struct node * quePop()
{
    return que->arr[que->tail++];
}

int queNotEmpty()
{
    if (que->head >que->tail)
    {
        return 1;
    }
    return 0;
}

void midPost(struct node *root)
{
    if (root == NULL)
    {
        return;
    }
    midPost(root->lchild);
    quePush(root);
    midPost(root->rchild);
}

//先根序遍历
void preOrder(struct node *T)
{
    if(T != NULL)
    {
        printf("%d ", T->data);
        preOrder(T->lchild);
        preOrder(T->rchild);
    }
    
}

void treeToLink()
{   
    int i;
    int num = que->head;
    printf("que num:%d\n", que->head);
    for( i=que->tail; i < num; i++)
    {
        if (i == 0)
        {
            que->arr[i]->lchild = NULL;
        }
        else
        {
            que->arr[i]->lchild = que->arr[i-1];
        }

        if (i == num-1)
        {
            que->arr[i]->rchild = NULL;
        }
        else
        {
            que->arr[i]->rchild = que->arr[i+1];
        }
    }
}

struct node *CreatTree()
{
    struct node *root = NULL;
    int tmp;

    scanf("%d", &tmp);
    if (tmp != 0)
    {
        root = malloc(sizeof(struct node));
        root->data = tmp;
        root->lchild = CreatTree();
        root->rchild = CreatTree();
    }
    return root;
}
void tree_to_link(struct node *root)
{
    struct node *p, *q;

    if (root != NULL)
    {
        p = root->lchild;
        if (p != NULL && p->rchild != NULL)
            p = p->rchild;
        q = root->rchild;
        if (q != NULL && q->lchild != NULL)
            q = q->lchild;
        tree_to_link(root->lchild);
        tree_to_link(root->rchild);
        if (p != NULL)
        {
            p->rchild = root;
            root->lchild = p;
        }
        if (q != NULL)
        {
            q->lchild = root;
            root->rchild = q;
        }
    }
}

int main()
{
    //freopen("/home/lintingbin/test.txt", "r", stdin);
    int n, i, tmp;
    struct node *tree, *head, *tm;
    que = malloc(sizeof(struct queue));
    while (scanf("%d", &n) != EOF)
    {
        for (i = 0; i < n; i++)
        {   
            memset(que, 0 ,sizeof(que));
            que->head = 0;
            que->tail = 0;
            head = tree = CreatTree();
            //tree_to_link(tree);
            preOrder(tree);
            printf("\n");
            midPost(tree);
            treeToLink();
            
            while (head != NULL && head->lchild != NULL)
                head = head->lchild;
            while (head != NULL)
            {
                tm = head;
                printf("%d ", head->data);
                head = head->rchild;
                free(tm);
            }
            
            printf("\n");
        }
    }
    free(que);
    return 0;
}