#include<stdio.h>
 
#define CIR(r) (r*r)

#define Y(n) ((n+1)*n)|
#define N 3

int main()
{
    int a =2;
    int b =3 ;
    int t = 0;
    t = CIR(a+b);
    t = 2*(N+Y(5+1);
    printf("%d\n", t);

    long *p = 0x100;
    p += sizeof(long);
    printf("%x\n", p);
    printf("%d\n",sizeof(long));

    return 0;
}