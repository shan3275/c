#ifndef LOCAL_SERVER_H
#define LOCAL_SERVER_H
#include "libsysbus.h"
#include "multiplex_io.h"
#include "list.h"

typedef struct
{
    char file_name[256];
    multiplex_io_t local_server_mio;
    multiplex_io_handle_t listen_io_handle;
    sysbus_handle_t *syslog_handle;
    list_t subscribe_client_list[65536];
} local_server_t;

local_server_t * local_server_start(const char *addr);
void local_server_stop(local_server_t * local_server);
//int local_server_clients_broadcast(local_server_t *local_server, unsigned char *data, int len);
#endif
