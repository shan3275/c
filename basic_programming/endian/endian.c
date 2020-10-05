#include<stdio.h>
typedef unsigned int uint32_t;
uint32_t Be32toh(uint32_t num)
{
    uint32_t data = 0;
	int a = 0x12345678;
	unsigned char *p=(unsigned char *)&a;
	if(0x78==*p)
	{
        data = ((num>>24)&0xff) | // move byte 3 to byte 0
            ((num<<8 )&0xff0000) | // move byte 1 to byte 2
            ((num>>8 )&0xff00) | // move byte 2 to byte 1
            ((num<<24)&0xff000000); // byte 0 to byte 3
	}
	else
	{
        data = num;
	}
    return data;
}
int main(void)
{
	int a = 0x12345678;
	unsigned char *p=(unsigned char *)&a;
	if(0x78==*p)
	{
	    printf("little end\n");
	}
	else
	{
	    printf("big end\n");
	}

    int num = Be32toh(0x12345678);
    printf("num:0x%x\n", num);
	return 0;

}

