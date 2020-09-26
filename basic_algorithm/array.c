#include <stdio.h>

int search_num_from_array(int arr[3][3], int num)
{
    int i,j;
    for (i = 0; i< 3; i++)
    {
        for (j = 0; j < 3; j++)
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
    int arr[3][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,9},
    };
    int num = 3;

    if (search_num_from_array(arr, num))
    {
        printf("Yes\n");
    }
    else
    {
        printf("No\n");
    }
    
}