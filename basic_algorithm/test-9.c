#include <stdio.h>
#include <string.h>

int calc(int depth)
{
    if (depth <=1)
    {
        return 1;
    }
    return depth*calc(depth-1);
}

unsigned long calc1(int depth)
{
    unsigned long num = 1;
    int i;
    for(i = 1; i <= depth; i++)
    {
        num = num * i;
    }
    return num;
}

int calcbitnum(unsigned long num)
{
    int i = 0;
    while (num !=0)
    {
        i++;
        num = num /10;
    }

    return i;
}

int main(int argc, char *argv[])
{
    int depth;
    unsigned long num=0;
    printf("long:%d\n", sizeof(long));
    if (scanf("%d", &depth))
    {
        num = calc1(depth);
        printf("num:%u\n",num);
        printf("bits: %d\n", calcbitnum(num));
    }
    return 0;
}