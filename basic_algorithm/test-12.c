#include <stdio.h>
#include <string.h>

#define N 1001

typedef int myType;

typedef struct treeNode
{
    myType element;
    struct treeNode *lchild;
    struct treeNode *rchild;
}Btree;

void createTree(Btree **T)
{
    myType data;
    scanf("%d", &data);
    //-1代表终端节点值
    if (data == -1)
    {
        *T = NULL;
    }
    else
    {
        *T = (Btree *)malloc(sizeof(struct treeNode));
        (*T)->element = data;
        printf("请输入%d的左孩子节点：", data);
        createTree(&((*T)->lchild));
        printf("请输入%d的右孩子节点：", data);
        createTree(&((*T)->rchild));
    }
}


//先根序遍历
void preOrder(Btree *T)
{
    if(T != NULL)
    {
        printf("%d ", T->element);
        preOrder(T->lchild);
        preOrder(T->rchild);
    }
    
}

//中根序遍历
void inOrder(Btree *T)
{
    if (T != NULL)
    {
        inOrder(T->lchild);
        printf("%d ", T->element);
        inOrder(T->rchild);
    }
    
}



//后根序遍历
void postOrder(Btree *T)
{
    if(T != NULL)
    {
        postOrder(T->lchild);
        postOrder(T->rchild);
        printf("%d ", T->element);
    }
    
}

int CanBiCreate;

Btree * reCreate(int *pre_order, int *mid_order, int count)
{
    Btree *root;
    int subCount = 0;
    if (count < 1)
    {
        return NULL;
    }

    while (*pre_order != *(mid_order+subCount))
    {
        subCount ++;
        if ( subCount >= count )
        {
            CanBiCreate = 0;
            return NULL;
        }
    }

    root = malloc(sizeof(Btree));
    root->element = *pre_order;
    root->lchild  = reCreate(pre_order+1, mid_order, subCount);
    root->rchild  = reCreate(pre_order+subCount+1, mid_order+1+subCount, count-subCount-1); 
    return root;
}


int main(int argc, char *argv[])
{
    Btree *tree;
    createTree(&tree);
    preOrder(tree);
    printf("\n");
    inOrder(tree);
    printf("\n");
    postOrder(tree);

    return 0;
}

int main1(int argc, char *argv[])
{
    int count, pre_order[N], mid_order[N];
    int i;
    Btree *tree;
    while (scanf("%d", &count) != EOF)
    {
        for (i = 0; i < count; i++)
        {
            scanf("%d", &pre_order[i]);
        }
        for( i = 0; i < count; i++)
        {
            scanf("%d", &mid_order[i]);
        }
        CanBiCreate = 1;
        tree = reCreate(pre_order, mid_order, count);
        if (CanBiCreate)
        {
            postOrder(tree);
        }
        else
        {
            printf("No");
        }
        printf("\n");
    }

    return 0;
}