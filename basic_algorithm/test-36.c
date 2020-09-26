
/*堆排序(大根堆)*/ 
#include <stdio.h>
 
/*注意：这个函数只会在调整被交换的位置为大根堆，未交换的分支不会处理，
所以不能将一个非大根堆二叉树的根结点传递过来让这个函数将其处理为大根堆*/
void heap_ajust(int *a, int i, int size)  /*a为堆存储数组，size为堆的大小*/
{
    int lchild = 2*i;       //i的左孩子节点序号 
    int rchild = 2*i +1;     //i的右孩子节点序号 
    int max = i; /*存放三个顶点中最大的数的下标*/
	int temp;
    if(i <= size/2)          //如果i是叶节点就不用进行调整 
    {
        if(lchild<=size && a[lchild]>a[max])
        {
            max = lchild;
        }    
        if(rchild<=size && a[rchild]>a[max])
        {
            max = rchild;
        }
        if(max != i)
        {
            temp = a[i]; /*交换a[i]和a[max]的值*/
			a[i] = a[max];
			a[max] = temp;
            heap_ajust(a, max, size); /*被交换的位置以前是大根堆，现在可能不是大根堆
			                            所以需要重新调整使其成为大根堆结构*/ 
        }
    }        
}
 
void heap_sort(int *a, int size) /*堆排序*/ 
{
    int i;
	int temp;
 
    for(i=size/2; i >= 1; i--) /*非叶节点最大序号值为size/2*/
    {
        heap_ajust(a, i, size); /*每个非叶子结点都需要调用这个函数*/   
    }    
    for(i=size; i >= 1; i--)
    {
        temp = a[1];
		a[1] = a[i];
		a[i] = temp; /*交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面*/ 
        heap_ajust(a, 1, i-1); /*重新调整堆顶节点成为大顶堆，只有被交换的分支才有可能不是大根堆*/
    }
} 
 
int main(int argc, char *argv[])
{
    int a[]={0, 16,20,3,11,17,8};
    int size = sizeof(a)/sizeof(int) -1;
	int i;
 
	printf("size = %d\n", size);
    heap_sort(a, size);
	printf("Sort over:"); 
    for(i=1;i <= size; i++)
        printf("%d ", a[i]);
    printf("\n");
 
    return 0;
}