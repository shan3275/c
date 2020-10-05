#include<stdio.h>

main()
{
    int n;
    scanf ("%d",&n);
    if (n > 0)
       printf("a > 0\n");  //后面会用IDA Pro把'a'改成'n'
    else
       printf("n < 0\n");
}
