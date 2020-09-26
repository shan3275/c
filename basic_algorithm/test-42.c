#include <stdio.h>

//约瑟夫环
int lastRemaining(unsigned int n, unsigned int m)
{
    if (n < 1 || m < 1)
        return -1;
    int last = 0, i;
    for (i = 2; i <= n; i++)
        last = (last + m) % i;
    return last + 1;
}

int main()
{
    //freopen("test.txt", "r", stdin);

    unsigned int n, m;
    while(scanf("%d", &n) != EOF && n != 0)
    {
        scanf("%d", &m);
        if (lastRemaining(n, m) != -1)
            printf("%u\n", lastRemaining(n, m));
    }
    return 0;
}