#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include<sys/time.h>

/*��tm�ṹ����ʱ�䣬�ṹtm��������:
struct  tm{
     int tm_sec;
     int tm_min;
     int tm_hour;
     int tm_mday;
     int tm_mon;
     int tm_year;
     int tm_wday;
     int tm_yday;
     int tm_isdst;
     };*/

/*  �õ�����ϵͳ��ʱ����ȷ���� */
/*int main(void)
{
    time_t first, second;
    first = time(NULL);  // Gets system time
    usleep(100000);
    second = time(NULL); // Gets system time again
    printf("The difference is: %f /seconds\n",difftime(second,first));
    return 0;
}*/

/* �õ���ǰϵͳʱ�䣬ת��Ϊ����ʱ�䣬��ת��Ϊ�ַ������
 * ���ص�ʱ���ַ�����ʽΪ������,��,��,Сʱ���֣���,��

int main()
 {
     time_t timer;
     struct tm *tblock;
     timer=time(NULL);
     tblock=localtime(&timer);
     printf("Local time is: %s",asctime(tblock));
     return 0;
}
*/
/*�õ�����ʱ��
 *�����ַ�����ʽ������,��,��,Сʱ:��:��,��
 */
int main()
{
    time_t t;
    time(&t);
    printf("Today's date and time: %s",ctime(&t));
 char date[32] = {0};
 struct timeval clock;
 struct tm *tm;
 gettimeofday(&clock, NULL);
 tm = localtime(&clock.tv_sec);
 strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
 printf("%s\n", date);
    return 0;

}


/*int main()

{

      struct  timeval    tv;
      struct  timezone   tz;
      gettimeofday(&tv,&tz);
      printf("tv_sec:%d\n",(int)tv.tv_sec);
      printf("tv_usec:%d\n",(int)tv.tv_usec);
      printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
      printf("tz_dsttime:%d\n",tz.tz_dsttime);
      return 0;
}

int delay(int time)

{
    int i,j;

    for(i =0;i<time;i++)
        for(j=0;j<5000;j++)
        {
        }

    return 0;

}

int main()

{
        struct  timeval start;
        struct  timeval end;

        unsigned  long diff;
        gettimeofday(&start,NULL);
       // delay(10);
    	usleep(1000000);
        gettimeofday(&end,NULL);
        diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
        printf("thedifference is %ld\n",diff);
        return 0;

}*/

/*
 *
 * 
 *
 char date[32] = {0};
 struct timeval clock;
 struct tm *tm;
 gettimeofday(&clock, NULL);
 tm = localtime(&clock.tv_sec);
 strftime(date, sizeof(date), "%Y%m%d%H%M%S", tm);
 printf("%s\n", date);
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */
