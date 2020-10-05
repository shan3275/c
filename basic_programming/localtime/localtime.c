/*****************************************************************************************
gmtime()�����ǽ�����ʱ��ת��Ϊ�����׼ʱ�䣨����������ʱ�䣩��
������һ��tm�ṹ�����������ʱ�䣬��localtime()�����ǽ�����ʱ
��ת��Ϊ����ʱ�䡣������localtime()�������й�������õı���ʱ��
��������׼ʱ����8��Сʱ:
local time:2009/01/26   3:44:17
UTC time:2009/01/25   19:44:17

linux��԰ (www.linuxview.net)����
License:GPL
E-mail:linuxdev@163.com
***************************************************************************************/

#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <time.h>
#include     <unistd.h>
#include     <sys/time.h>
#include     <sys/times.h>

int main()
{
    time_t the_time;
    struct tm *tp;
    time(&the_time);
    tp=localtime(&the_time);

    printf("Local time:%d/%.2d/%.2d   %d:%.2d:%.2d\n",
        tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,
        tp->tm_hour,tp->tm_min,tp->tm_sec);

    tp=gmtime(&the_time);
    printf("UTC time:%d/%.2d/%.2d   %d:%.2d:%.2d\n",
        tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,
        tp->tm_hour,tp->tm_min,tp->tm_sec);

    return 0;
}
