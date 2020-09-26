#include <stdio.h>
#include <string.h>
#define N 1000

int main(int argc, char *argv[])
{
    int arr[N];
    int i=0;
    int j;
    while (scanf("%d", &arr[i++]) != EOF)
    {
        if (arr[i-1] == -1)
        {
            for(j = i-1; j >0; j--)
            {
                printf("%d\n", arr[j-1]);
            }
            return 0;
        }
    }
    

    return 0;
}