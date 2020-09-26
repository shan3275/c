#include <stdio.h>
#include <string.h>


int callcc(int num)
{
    int i;
    int result = 1;
    if (num == 0)
    {
        return 1;
    }

    for( i = 0; i< num; i++)
    {
        result = result *2;
    }

    return result;
}

int calc(int num)
{
    int i;
    int result = 0;
    for (i = 0; i < num; i++ )
    {
        //result += callcc(i);
        result |= (1<<i);
    }

    return result;
}

int main(int argc, char *argv[])
{
    int num;
    if (scanf("%d", &num))
    {
        printf("result: %d\n", calc(num));
    }
}