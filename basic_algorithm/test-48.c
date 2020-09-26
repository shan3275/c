#include <stdio.h>

void swap_buff(char *buff)
{
    int i,j;
    char tmp;
    int len = strlen(buff);
    for (i=len-1,j=0; i>=len/2; i--,j++)
    {
        tmp = buff[i];
        buff[i] = buff[j];
        buff[j] = tmp;
    }
}

int main(int argc, char *argv[])
{
    char buff[100];
    while (gets(buff) != NULL)
    {
        swap_buff(buff);
        printf("%s\n",buff);
    }
    
    return 0;
}