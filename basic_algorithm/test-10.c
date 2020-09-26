#include <stdio.h>
#include <string.h>

#define N 100000

int calc(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }

    int array[N] = {0};
    array[0] = 1;
    int i=0,j=0;
    int valid = 1;

    for ( i = 2; i <= n; i++)
    {
        long carry = 0;
        for ( j = 0; j < valid; j++ )
        {
            long multipleResult = array[j] * i + carry;
            array[j] = (unsigned int)(multipleResult %10);
            carry = multipleResult / 10;
        }

        while (carry != 0)
        {
            array[valid++] = (unsigned int)(carry %10);
            carry = carry/10;
        }
    }

    return valid;

}

int main(int argc, char *argv[])
{
    int depth;
    unsigned num=0;
    if (scanf("%d", &depth))
    {
        num = calc(depth);
        printf("num:%u\n",num);
    }
    return 0;
}