#include <stdio.h>
#define N 1000
int arr[N][N];
int m,n,num;

int search_num_from_array(int x, int y, int num)
{
    int i,j;
    for (i = 0; i< x; i++)
    {
        for (j = 0; j < y; j++)
        {
            if (arr[i][j] == num)
            {
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i,j;
    while(scanf("%d %d", &m, &n) != EOF)
    {
        if (scanf("%d", &num))
        {
            for(i = 0; i < m; i++)
            {
                for(j = 0; j < n; j++)
                {
                    scanf("%d", &arr[i][j]);
                }
            }

            if (search_num_from_array(m,n,num))
            {
                printf("Yes\n");
            }
            else
            {
                printf("No\n");
            }
        }
    }
    printf("over\n");    
}