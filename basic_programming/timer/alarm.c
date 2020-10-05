#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include<sys/time.h>
#include <signal.h>

/**
 *  alarm
 *  真真真真真真 alarm() � signal() 真� 
 *  unsigned int alarm(unsigned int seconds) 
 *  � 真SIGALRM真真真真真�seconds真真真真真�SIGALRM真真真真真�
 *  真真alarm真真真� alarm()真真真�
 *  真真seconds真真真真真真真真真真
 *  真真alarm真真真真真真真真真 真真真真真真真真真�0�
 **/

void sigalrm_fn(int sig)
{
    /* Do something */
    printf("alarm!/n");
    alarm(2);
    return;
}

int main(void) 
{
    signal(SIGALRM, sigalrm_fn);
    alarm(2);

    /* Do someting */
    while(1) pause();
}

