#include <stdio.h>
#define N 1000
int arr[N][N];
int m,n,num;

int reverse(int x)
{
    long num=0;    
    while( x !=0 )
    {
        num = num *10;
        num += x%10;
        x = x/10;
    }

    return (int)num;
}

int main(int argc, char *argv[])
{
    int i,j;
    int m;
    while(scanf("%d", &m) != EOF)
    {
        printf("%d\n", reverse(m));
    }
    printf("over\n");    
}