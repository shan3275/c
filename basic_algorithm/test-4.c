/*
    https://github.com/lintingbin2009/C-language/tree/master/%E5%89%91%E6%8C%87offer/%E9%9D%A2%E8%AF%95%E9%A2%9804%E2%80%94%E2%80%94%E6%9B%BF%E6%8D%A2%E7%A9%BA%E6%A0%BC
    面试题04——替换空格
 */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i,j=0;
    char buff[100];
    char new_buff[100];
    while(gets(buff) != NULL)
    {
        memset(new_buff, 0 ,sizeof(new_buff));
        j = 0;
        for (i = 0; i< strlen(buff); i++)
        {
            if (buff[i] == ' ' &&  buff[i+1] != ' ')
            {
                new_buff[j++] = '%';
                new_buff[j++] = '2';
                new_buff[j++] = '0';
            }
            if (buff[i] != ' ')
            {
                new_buff[j++] = buff[i];
            }
        }
        printf("%s\n", new_buff);
        memset(buff, 0, sizeof(buff));
    }   

}