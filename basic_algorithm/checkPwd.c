#include<stdio.h>
#define N 100
#define MIN_LENGTH 9
#include<string.h>
int main()
{
    char buff[N]={0};   
    while(gets(buff)!= NULL) {
        unsigned char flag = 0; // bit 0 for upper case, bit 1 for lower case, bit 2 for digit, bit 3 for special character
        unsigned char num = 0;
        printf("%s",buff);

        // check length
        if(strlen(buff) < MIN_LENGTH) {
            printf("length is less than 9, please enter again\n");
            continue;
        }

        // check character number and special character
        for(int i = 0; i < strlen(buff); i++) {
            if(buff[i] >= 'A' && buff[i] <= 'Z') {
                flag = flag | 0x01;
            }else if (buff[i] >= 'a' && buff[i] <= 'z') {
                flag = flag | 0x02;
            }else if(buff[i] >= '0' && buff[i] <= '9') {
                flag = flag | 0x04;
            }else {
                flag = flag | 0x08;
            }
        }
        while(flag) {
            if(flag & 0x01) {
                num++;
            }
            flag = flag >> 1;
        }
        if (num < 3) {
            printf("character type is not more than 3, please enter again\n");
            continue;
        }
        
        //check repeat character
        for(int i = 0; i < strlen(buff); i++) {
            for(int j = i + 1; j < strlen(buff); j++) {
                if(buff[i] == buff[j]) {
                    if ((i+1 < strlen(buff)) && (j+1 < strlen(buff)) && (i+2 < strlen(buff)) && (j+2 < strlen(buff))) {
                        if(buff[i+1] == buff[j+1] && buff[i+2] == buff[j+2]) {
                            printf("repeat string length is more than 3, please enter again\n");
                            goto next;
                        }
                    }
                }
            }
        }
        printf("password is valid\n");
        printf("password is %s\n", buff);
        next:
        memset(buff, 0, N);
    }
    return 0;
}
