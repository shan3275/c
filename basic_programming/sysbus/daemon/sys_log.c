#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "libsysbus.h"
#include "sys_log.h"

static char * syslog_level_string(unsigned char level)
{
    char *lvl_str[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    if (level > 4)
    {
        level = 4;
    }
    return lvl_str[level];
}

static void sys_log_pkt_handle(unsigned short pkt_id, unsigned char *data, int data_len, void *param)
{
    time_t t;
    struct tm *ti;
    char buf[256];
    int fd, len;
    t = time(NULL); 
    ti = localtime(&t);	
    if ((pkt_id == SYS_LOG_MSG) && (data_len >= 2))
    {
        data[data_len-1] = '\0';
        snprintf(buf, sizeof(buf), "%s/%d-%02d-%02d.log", (char *)param, (1900+ti->tm_year), (1+ti->tm_mon), ti->tm_mday);
        fd = open(buf, O_WRONLY|O_CREAT|O_APPEND, 0666);
        if (fd >= 0)
        {
            snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d %s ",(1900+ti->tm_year), (1+ti->tm_mon), ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec, syslog_level_string(data[0]));
            len = strlen(buf);
            if (len == write(fd, buf, len))
            {
                len = write(fd, &data[1], data_len - 2);
            }
            close(fd);
        }
        //printf("[%d-%d-%d %d:%d:%d] %s\n",  (1900+ti->tm_year), (1+ti->tm_mon), ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec, data);
    }
}
sysbus_handle_t *syslog_start(char* sysbus_file, char *log_file_dir)
{
    sysbus_handle_t *sysbus = NULL;
    unsigned short subscribe_id[1];
    sysbus = sysbus_connect(sysbus_file, "syslog", sys_log_pkt_handle, log_file_dir);
    if (sysbus != NULL)
    {
        subscribe_id[0] = SYS_LOG_MSG;
        if (1 != sysbus_msg_subscribe(sysbus, subscribe_id, 1))
        {
            sysbus_disconnect(sysbus);
            sysbus = NULL;
        }
        
    }
    return sysbus;
}