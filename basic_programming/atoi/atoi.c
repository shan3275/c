/*������: atoi
��  ��: ���ַ���ת���ɳ�������
��  ��: int atoi(const char *nptr);
������:*/
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
   int n;
   char *str = "   12345M_116";

   n = atoi(str);
   printf("string = %s integer = %d\n", str, n);
   return 0;
}
