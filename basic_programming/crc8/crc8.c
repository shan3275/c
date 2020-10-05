#include <stdlib.h>
#include <stdio.h> 

/* ���� 1*/
/* ������: CRC8 * 
 * ����ʽ��x8+x2+x1+1
 * ��  ��:CRCУ��
 * ��  ��:unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
 * ������ptr�Ǵ�У������������ʼ��ַ��len�Ǵ�У�����������ֽڳ��ȣ�key�����ɶ���ʽ�ļ��ʽ
 * ���⣺��򵥣��ٶ�������ռ���ڴ�����
 * ������:*/
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
{
    unsigned char i;
    unsigned char crc=0;
    while(len--!=0)
    {
	for(i=0x80; i!=0; i/=2)
	{
            if((crc&0x80)!=0) {crc*=2; crc^=key;} /* ��ʽCRC ����2 ����CRC */
	    else crc*=2;
	    if((*ptr&i)!=0) crc^=key; /* �ټ��ϱ�λ��CRC */
	}
	ptr++;
    }
	return(crc);
}

/* ���� 2 ���ֽڲ��*/
/* ��������mK_cRctable
 * ����  ������0-15��Ӧ��CRCУ����,��ʵ���Ǽ�����㷨
 *
 * */
unsigned char CRC8HalfTable[16];
void mK_cRctable(unsigned char * cRctable_8, unsigned char key)
{
    unsigned char cRc_8=0;
    unsigned char i,j;

    for(i=0;i<16;i++)
    {
	cRc_8 = i;
	for(j=8;j>0;j--)
	{
	if(cRc_8&0x80) 
	cRc_8=(cRc_8<<1)^key; 
	else
	cRc_8<<=1; 
	}
	cRctable_8[i] = cRc_8;
    }
}
/* ��������GetCrc_8
 * ����  ������������* pData��16λCRCУ���룬����������ΪnLength
 * */
unsigned char GetCrc_8(unsigned char * ptr, int len, unsigned char * PMBUS_CRC8_Table)
{
    unsigned char crc;
    unsigned char da;

    crc=0;
    while(len--!=0)
    {
        da=crc/16; /* �ݴ�CRC �ĸ���λ */
	crc<<=4; /* CRC ����4 λ���൱��ȡCRC �ĵ�12 λ��*/
	crc^=PMBUS_CRC8_Table[da^(*ptr/16)]; /* CRC �ĸ�4 λ�ͱ��ֽڵ�ǰ���ֽ���Ӻ������CRC��Ȼ�������һ��CRC������ */
	da=crc/16; /* �ݴ�CRC �ĸ�4 λ */
	crc<<=4; /* CRC ����4 λ�� �൱��CRC �ĵ�12 λ�� */
	crc^=PMBUS_CRC8_Table[da^(*ptr&0x0f)]; /* CRC �ĸ�4λ�ͱ��ֽڵĺ���ֽ���Ӻ������CRC��Ȼ���ټ�����һ��CRC������ */
	ptr++;
    }
    return(crc);
}

/* ����3 �ֽڲ�� */

/* ��������mK_cRctable_256
 * ����  ������0-255��Ӧ��CRCУ����,��ʵ���Ǽ�����㷨
 *
 * */
unsigned char CRC8Table[256];
void mK_cRctable_256(unsigned char * cRctable_8, unsigned char key)
{
    unsigned char cRc_8=0;
    unsigned char j;
    unsigned int i;

    for(i=0;i<256;i++)
    {
	cRc_8 = i;
	for(j=8;j>0;j--)
	{
	if(cRc_8&0x80) 
	cRc_8=(cRc_8<<1)^key; 
	else
	cRc_8<<=1; 
	}
	cRctable_8[i] = cRc_8;
    }
}

unsigned char cal_crc(unsigned char *ptr, unsigned char len)
{
    unsigned char crc=0;
    while(len--!=0)
    {
        crc =CRC8Table[crc ^ *ptr++];
    }
    return(crc);
} 

int main(void)
{
   int n;
   int i;
   unsigned char str[9] = {0x23,0x23,0x23,0x34,0x45,0xab,0x45,0x98,0xcb};

   n = CRC8(str, 9, 7); 
   printf("Method 1 CRC-8 = 0x%.2x\n", n); 
   n = 0;
   mK_cRctable(CRC8HalfTable,7);
   for(i=0; i< 16; i++)
   {
	   printf("0x%.2x  ", CRC8HalfTable[i]);
   }
   printf("\n");
   n = GetCrc_8(str, 9, CRC8HalfTable);
   printf("Method 2 CRC-8 = 0x%.2x\n", n); 

   n = 0;
   mK_cRctable_256(CRC8Table, 7);
   for(i=0; i<=255; i++)
   {
	   if(i%16 == 0)
		   printf("\n");
	   printf("0x%.2x,", CRC8Table[i]);
   }
   n = cal_crc(str, 9);
   printf("\n");
   printf("Method 3 CRC-8 = 0x%.2x\n", n); 
   return 0; 
}
