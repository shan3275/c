#include <stdio.h>

#define N 1000
typedef struct bTree_t
{
    int element;
    struct bTree_t *left;
    struct bTree_t *right;
}bTree;


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

int main(int argc, char *argv[])
{
    int n,tmp;
    char op;
    int i;
    bTree *input[N];
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
        mirror(input[0]);
        preOrder(input[0]);
        printf("\n");
    }
    
    return 0;
}