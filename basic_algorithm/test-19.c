#include <stdio.h>

int main(int argc, char *argv[])
{
    int num;
    int i;
    int loop=10;
    while (scanf("%d", &num) != EOF)
    {
        loop = 1;
        for (i = 0; i<num; i++)
        {
            loop *=10; 
        }
        for (i = 1 ; i < loop; i++)
        {
            printf("%d\n", i);
        }
    }
    
    return 0;
}