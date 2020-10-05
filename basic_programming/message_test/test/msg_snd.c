/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-08-10 23:36
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
    // 1. 创建一个消息队列，用 key = 123 来唯一表示这个队列    
    int msg_id = msgget(123, IPC_CREAT | 0666);
    if (msg_id != -1) {

        // 2. 初始化要发生的消息
        struct msgbuf mybuf;
        mybuf.mtype = 1;
        strcpy(mybuf.mtext, "I'm send process.\n");

        // 3. 发送消息
        for(;;)
        {
            if (msgsnd(msg_id, &mybuf, sizeof(mybuf.mtext), 0)) 
                printf("success\n");
            else
                perror("msgsnd:");
            
        }
    } else {
        perror("msgget:");
    }

    return 0;
}

