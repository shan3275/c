#include <stdio.h>

#define N 11

void sort(char *arr)
{
    int i,j;
    int tmp;
    int len = strlen(arr);
    for (i = 0; i < len; i++)
    {
        for (j=i+1; j < len; j++)
        {
            if (arr[i] > arr[j])
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp; 
            }
        }
    }

}


void sort1(char *str)
{
    int chart[200], i, j;
    
    int len = strlen(str);
    memset(chart, 0, 200*sizeof(int));
    for (i = 0; i < len; i++)
        chart[str[i]]++;
    for (i = j = 0; i < 200; i++)
        while (chart[i]-- != 0)
            str[j++] = i;
}

char result[N];
int used[N] = {0};

int isRepeat(int *num, int len, int rep)
{
    int i;
    for (i = 0; i < len; i++)
        if (rep == num[i])
            return 1;
    return 0;
}

void pailie(char *arr, int len, int index)
{
    int i,repeat[N] = {0};
    if(len==index)
    {
        printf("%s\n", result);
        return;
    }
    for (i = 0; i < len; i++)
    {
        if (used[i] == 1)
        {
            continue;
        }
        result[index] = arr[i];
        if (isRepeat(repeat, i, arr[i]))
            continue;
        repeat[i] = arr[i];
        used[i] = 1;
        pailie(arr, len, index+1);
        used[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    char arr[N]={0};
    int len;

    //while (gets(arr) != NULL)
    while (scanf("%s",arr) != EOF)
    {
        printf("输入为：%s\n", arr);
        //sort(arr);
        sort1(arr);
        len = strlen(arr);
        result[len] = '\0';
        printf("输入为：%s\n", arr);
        pailie(arr, len, 0);
    }
    
}