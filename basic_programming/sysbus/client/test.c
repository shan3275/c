#include <stdio.h>
#include <unistd.h>
#include "libsysbus.h"

#define CAMERA_CMD_MSG (0x100)
#define CAMERA_STS_MSG (0x101)

void msg_rx_proc(unsigned short pkt_id, unsigned char *data, int data_len, void *param)
{
    printf("message %d received, length = %d, data= %s\n\n", pkt_id, data_len, data);
}

int main(int argc, char *argv[])
{
    sysbus_handle_t *h;
    unsigned short subscribed_pkt_ids[1] = {CAMERA_CMD_MSG};
    unsigned char test_data[4] = {'a','b','c','d'};
    if (argc < 2) {
        printf("USAGE: test unix_socket_path\n");
        return 0;
    }
    h = sysbus_connect(argv[1], "demo", msg_rx_proc, NULL);
    if (h != NULL)
    {
        sysbus_msg_subscribe(h, subscribed_pkt_ids, 1);
        sysbus_msg_send(h, CAMERA_STS_MSG, test_data, 4);
        for(int i = 0; i < 10; i++)
        {
            sysbus_msg_send(h, CAMERA_CMD_MSG, test_data, 4);
            syslog_write(h, SYSLOG_LEVEL_INFO, "hello log.\n");
            sleep(5);
        }
        sysbus_disconnect(h);
    }
    else
    {
        printf("sysbus_connect() fail.\n");
    }
    return 0;
}