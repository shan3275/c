/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-08-13 17:57
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

struct msgbuf {
    long mtype;
    char mtext[255];
};

void msg_snd(int *couter);
void msg_rcv(int *couter);


int main(int argc, char *argv[])
{
    pthread_t thrd1, thrd2;
    int ret;

    // 1. 创建一个消息队列，用 key = 123 来唯一表示这个队列
    int msg_id = msgget(123, IPC_CREAT | 0666);
    /* Create the first thread */
    ret = pthread_create(&thrd1, NULL, (void *)msg_snd, (void *)&msg_id);
    if (ret) {
        perror("pthread_create: task1");
        exit(EXIT_FAILURE);
    }

    /* Create the second thread */
    ret = pthread_create(&thrd2, NULL, (void *)msg_rcv, (void *)&msg_id);
    if (ret) {
        perror("pthread_create: task2");
        exit(EXIT_FAILURE);
    }

    printf("pthread task  id: %d\n", (int)pthread_self());
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);

    exit(EXIT_SUCCESS);
}

void msg_snd(int *counter)
{
    int msg_id = *counter;
    printf("pthread task1 id: %d\n", (int)pthread_self());
    if (msg_id != -1) 
    {
        // 2. 初始化要发生的消息
        struct msgbuf mybuf;
        mybuf.mtype = 1;
        strcpy(mybuf.mtext, "I'm send process.\n");

        // 3. 发送消息
        for(;;)
        {
            //sleep(10);
            if (msgsnd(msg_id, &mybuf, sizeof(mybuf.mtext), IPC_NOWAIT))
                perror("msgsnd: fail");
            else
                printf("snd success\n");
        }
    } else {
        perror("msgget:");
    }
}


void msg_rcv(int *counter)
{
    int msg_id = *counter;
    printf("pthread task2: %d\n", (int)pthread_self());
    if (msg_id != -1) 
    {
        struct msgbuf mybuf;
        // 2. 接收第一条消息，存到 mybuf 中
        for(;;)
        {
            //sleep(20);
            if (msgrcv(msg_id, &mybuf, sizeof(mybuf.mtext), 0, IPC_NOWAIT) != -1) {
                printf("read success: %s\n", mybuf.mtext);
                // 3. 接收完就删除这个消息队列
            } else {
                perror("msgrcv:fail");
            }
        }
        if (msgctl(msg_id, IPC_RMID, 0) != -1)
            printf("delete msg success\n");

    } else {
        perror("msgget:");
    }
}


