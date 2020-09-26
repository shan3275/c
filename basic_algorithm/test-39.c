#include <stdio.h>
#define N 100001

int input[N];


int calc(int num)
{
    if (num ==1)
    {
        return 1;
    }
    return num + calc(num-1);
}


/*
arr:    -2 1 -3 4 -1 2 1 -5 4
        0  1  2 3 4  5 6  7 8
        6 3 6
        4 -1 2 1
*/
int main()
{
    //freopen("/home/lintingbin/test.txt", "r", stdin);
    int n, i, start, end, sum, start_test, end_test, sum_test;

    while (scanf("%d", &n) != EOF)
    {
        printf("rv: %d\n", calc(n));
    }



    return 0;
}