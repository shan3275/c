#include <stdio.h>
#include <unistd.h>
#include "local_server.h"
#include "sys_log.h"


int main(int argc, char *argv[])
{
    int ret;
#if 0
    sysbus_handle_t *h;
#endif
    local_server_t *serv;
    if (argc >= 2)
    {
        serv = local_server_start(argv[1]);
        if (serv != NULL)
        {
#if 0
            if (argc > 2)
            {
                h = syslog_start(argv[1], argv[2]);
                if (NULL != h)
                {
                    serv->syslog_handle = h;
                    ret = 0;
                    while (1)
                    {
                        sleep(10);
                    }
                }
                else
                {
                    ret = -3;
                    printf("ERROR: syslog_start(%s, %s) fail.\n", argv[1], argv[2]);
                }
            }
            else
            {
                ret = 0;
                printf("WARNING: syslog service is not started due to log_dir_path is not specified in prompt.\n");
                while (1)
                {
                    sleep(10);
                }
            }
#else
            while (1)
            {
                sleep(10);
            }
#endif
        }
        else
        {
            ret = -2;
            printf("ERROR: local_server_start(%s) fail.\n", argv[1]);
        }
        
    }
    else
    {
        ret = -1;
        printf("USAGE: sysbusd unix_socket_path [log_dir_path]\n");
    }
    return ret;
}
