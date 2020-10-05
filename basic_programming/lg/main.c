#include <stdio.h>

int lg(int N)
{
    int i;
    for( i = 0; N > 0; i++, N /=2);
    return i;
}

int main(int argc, char *argv[])
{
    int i, N;
    for ( i = 1, N = 10; i <= 6; i++, N *=10)
    {
        printf("%7d %2d %9d\n", N, lg(N), N*lg(N));
    }
    return 0;
}