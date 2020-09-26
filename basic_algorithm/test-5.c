#include <stdio.h>
#include <string.h>

#define N 1000

/*
面试题04——替换空格 推荐方法
*/
int main(int argc, char *argv[])
{
    char buff[N];
    int i;
    int count;
    int len;
    while (gets(buff) != NULL)
    {
        count = 0;
        len = strlen(buff);
        printf("len=%d\n", len);
        for (i = 0; i < strlen(buff); i++)
        {
            if ( buff[i] == ' ')
            {
                count = count + 2;
            }
              
        }
        printf("count=%d\n", count);

        for (i = len; i >= 0; i--)
        {
            if (buff[i] != ' ')
            {
                buff[i+count] = buff[i];
            }
            else
            {
                buff[i+count]   = '0';
                buff[i+--count] = '2';
                buff[i+--count] = '%';
            }
        }
        printf("%s\n", buff);
        memset(buff, 0, N);
    }

    return 0;
}