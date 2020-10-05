/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-08-10 23:37
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct msgbuf {
        long mtype;
            char mtext[255];
};

int main() {
    // 1. 获取消息队列
    int msg_id = msgget(123, IPC_CREAT | 0666);
    if (msg_id != -1) {
        struct msgbuf mybuf;
        // 2. 接收第一条消息，存到 mybuf 中
        for(;;)
        {
            if (msgrcv(msg_id, &mybuf, sizeof(mybuf.mtext), 0, IPC_NOWAIT) != -1) { 
                printf("read success: %s\n", mybuf.mtext);
                // 3. 接收完就删除这个消息队列
            } else {
                perror("msgsnd:");
            }
        }
        if (msgctl(msg_id, IPC_RMID, 0) != -1)
            printf("delete msg success\n");

    } else {
        perror("msgget:");
    }

    return 0;
}

