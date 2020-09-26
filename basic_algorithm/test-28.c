#include <stdio.h>

#define N 1000
typedef struct bTree_t
{
    int element;
    struct bTree_t *left;
    struct bTree_t *right;
}bTree;

typedef struct queue_t
{
    bTree *arr[N];
    int head;
    int tail;
}queue;

queue *que;

void quePush(bTree *node)
{
    que->arr[que->head++] = node; 
}

bTree * quePop()
{
    return que->arr[que->tail++];
}

int queNotEmpty()
{
    if (que->head > que->tail)
    {
        return 1;
    }
    return 0;
}

//从左到右，从上到下打印二叉树
void printBtree(bTree *node)
{
    if (node == NULL)
    {
        return;
    }
    quePush(node);
    while (queNotEmpty())
    {
        bTree * tmp = quePop();
        printf("%d ", tmp->element);
        if (tmp->left)
        {
            quePush(tmp->left);
        }
        if (tmp->right)
        {
            quePush(tmp->right);
        }
    }
    
}

//前序打印二叉树
void preOrder(bTree *node)
{
    if (node == NULL)
    {
        return ;
    }
    printf("%d ", node->element);
    preOrder(node->left);
    preOrder(node->right);
}

//镜像二叉树
void mirror(bTree *node)
{
    bTree * leftNode;
    bTree * rightNode;
    if (node == NULL)
    {
        return;
    }

    leftNode   = node->left;
    rightNode  = node->right;
    node->left  = rightNode;
    node->right = leftNode;
    mirror(node->left);
    mirror(node->right);

}

//计算二叉树节点数
int getNodeNum(bTree *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return getNodeNum(node->left) + getNodeNum(node->right) +1;
}

//结算二叉树深度
int getNodeDepth(bTree *node)
{
    if (node == NULL)
    {
        return 0;
    }
    int leftDepth  = getNodeDepth(node->left);
    int rightDepth = getNodeDepth(node->right);
    return leftDepth > rightDepth ? (leftDepth +1) :(rightDepth +1);
}

int main(int argc, char *argv[])
{
    int n,tmp;
    char op;
    int i;
    bTree *input[N];
    que = malloc(sizeof(queue));
    while (scanf("%d", &n) != EOF)
    {
        for (i = 0; i < n; i++)
        {
            input[i] = malloc(sizeof(bTree));
            memset(input[i], 0, sizeof(bTree));
            scanf("%d", &tmp);
            input[i]->element = tmp;
        }

        for( i = 0; i < n; i++)
        {
            scanf(" %c", &op);
            switch (op)
            {
            case 'd':
                scanf("%d", &tmp);
                input[i]->left  = input[tmp-1];
                scanf("%d", &tmp);
                input[i]->right = input[tmp-1];
                break;
            case 'l':
                scanf("%d", &tmp);
                input[i]->left = input[tmp-1];
                break;
            case 'r':
                scanf("%d", &tmp);
                input[i]->right = input[tmp-1];
                break;
            case 'z':
                input[i]->right = NULL;
                input[i]->left  = NULL;
                break;
            default:
                break;
            }
        }

        //打印
        preOrder(input[0]);
        printf("\n");
        memset(que, 0,sizeof(queue));
        printBtree(input[0]);
        printf("\n");
        printf("node number: %d\n", getNodeNum(input[0]));
        printf("node depth:  %d\n", getNodeDepth(input[0]));
    }
    free(que);
    
    return 0;
}