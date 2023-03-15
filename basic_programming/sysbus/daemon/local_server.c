#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<sys/un.h>
#include <sys/time.h>
#include "local_server.h"

typedef struct
{
    char client_name[32];
    unsigned char rx_buf[65540];
    unsigned int rx_idx;
    multiplex_io_handle_t io_handle;
    local_server_t *server;
    list_t list;
    list_t ref_list;
} client_runtime_t;

typedef struct local_server
{
    client_runtime_t * client;
    list_t list;
    list_t ref_list;
} client_runtime_ref_list_t;

//void print_hex(unsigned char *data, unsigned int len);

void print_hex(unsigned char *data, unsigned int len)
{
    int i;
    for (i=0; i<len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


/*
static int listen_fd_err_callback(multiplex_io_t io_handle, void *param)
{
    unsigned int event = *((unsigned int *)param);
    printf("listen_fd_err_callback(0x%X)\n", event);
    return 0;
}
*/

static client_runtime_ref_list_t * subscribe_pkt(client_runtime_t *cli, list_t *list)
{
    client_runtime_ref_list_t *cli_ref;
    list_t *n;
    n = list->next;
    while (n != list)
    {
        cli_ref = list_entry(n, client_runtime_ref_list_t, list);
        if (cli_ref->client == cli)
        {
            break;
        }
        n = n->next;
    }
    if (n == list)
    {
        cli_ref = malloc(sizeof(client_runtime_ref_list_t));
        if (cli_ref != NULL)
        {
            cli_ref->client = cli;
            list_init(&cli_ref->ref_list);
            list_init(&cli_ref->list);
            list_insert_after(list, &cli_ref->list);
            list_insert_after(&cli->ref_list, &cli_ref->ref_list);
        }
    }
    return cli_ref;
}

static int unsubscribe_pkt(client_runtime_t *cli, list_t *list)
{
    int ret = 0;
    client_runtime_ref_list_t *cli_ref;
    list_t *n;
    n = list->next;
    while (n != list)
    {
        cli_ref = list_entry(n, client_runtime_ref_list_t, list);
        n = n->next;
        if (cli_ref->client == cli)
        {
            ret++;
            list_remove(&cli_ref->list);
            list_remove(&cli_ref->ref_list);
            free(cli_ref);   
        }
    }
    return ret;
}

static int dispatch_pkt(unsigned char *data, int len, list_t *list)
{
    int ret = 0;
    int rv;
    list_t *n = list->next;
    client_runtime_ref_list_t *cli_ref;
    while(n != list)
    {
        cli_ref = list_entry(n, client_runtime_ref_list_t, list);
        rv = multiplex_io_write(cli_ref->client->server->local_server_mio, cli_ref->client->io_handle, data, len);
        if (0 != rv)
        {
            syslog_write(cli_ref->client->server->syslog_handle, SYSLOG_LEVEL_ERROR, "%s dispatch_pkt(): multiplex_io_write(%d) fail, returns %d.\n",cli_ref->client->client_name, len, rv);
            //printf("dispatch_pkt(): multiplex_io_write() fail.\n");
        }
        n = n->next;
        ++ret;
    }
    return ret;
}
static int com_fd_rd_callback(multiplex_io_handle_t io_handle, void *param)
{
    int ret = 0;
    unsigned int idx;
    unsigned int base_idx;
    unsigned short pkt_size;
    unsigned short pkt_id;
    unsigned short tmp_u16;
    //unsigned char pkt_value;
    unsigned char *rx_buf;
    int i;
    int fd;
    client_runtime_t *cli = (client_runtime_t *)param;
    client_runtime_ref_list_t *cli_ref;
    idx = cli->rx_idx;
    rx_buf = cli->rx_buf;
    
    ret = multiplex_io_read(cli->server->local_server_mio, io_handle, &rx_buf[idx], sizeof(cli->rx_buf) - idx);
    if (ret > 0)
    {
        idx += ret;
        base_idx = 0;
        while (idx > 4 + base_idx)
        {
            pkt_size = (rx_buf[base_idx] | (rx_buf[base_idx + 1]<<8));
            pkt_id = (rx_buf[base_idx + 2] | (rx_buf[base_idx + 3]<<8));
            if (base_idx + pkt_size + 2 <= idx)
            {
                if (0 != pkt_id)
                {
                    dispatch_pkt(&rx_buf[base_idx], pkt_size+2, &cli->server->subscribe_client_list[pkt_id]);
                }
                else if (pkt_size >= 4)
                {
                    tmp_u16 = (rx_buf[base_idx + 4] | (rx_buf[base_idx + 5]<<8));
                    
                    switch(tmp_u16)
                    {
                        case 0x0000:
                            i = pkt_size - 4;
                            if (i > sizeof(cli->client_name) - 1)
                            {
                                i = sizeof(cli->client_name) - 1;
                            }
                            memcpy(cli->client_name, &rx_buf[base_idx + 6], i);
                            cli->client_name[i] = '\0';
                            syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_INFO,"%s connected to sysbus.\n",cli->client_name);
                            break; 
                        case 0x0001:
                            i = 4;
                            // subscribe pkt(s)
                            while (i < (pkt_size & 0xFFFE))
                            {
                                tmp_u16 = (rx_buf[base_idx + 2 + i] | (rx_buf[base_idx + 3 + i]<<8));
                                if (NULL == subscribe_pkt(cli, &cli->server->subscribe_client_list[tmp_u16]))
                                {
                                    syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_ERROR,"%s subscribe_pkt(%d) fail.\n",cli->client_name, tmp_u16);
                                }
                                else
                                {
                                    syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_INFO,"%s subscribe 0x%X pkt.\n",cli->client_name, tmp_u16);
                                }
                                i += 2;
                            }
                            break;
                        case 0x0002:
                            i = 4;
                            // unsubscribe pkt(s)
                            while (i < (pkt_size & 0xFFFE))
                            {
                                tmp_u16 = (rx_buf[base_idx + 2 + i] | (rx_buf[base_idx + 3 + i]<<8));
                                if (tmp_u16 != 0)
                                {
                                    if (0 == unsubscribe_pkt(cli, &cli->server->subscribe_client_list[tmp_u16]))
                                    {
                                        syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_ERROR,"%s unsubscribe_pkt(%d) fail.\n",cli->client_name, tmp_u16);
                                    }
                                    else
                                    {
                                        syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_INFO,"%s unsubscribe 0x%X pkt.\n",cli->client_name, tmp_u16);
                                    }
                                }
                                i += 2;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    // do nothing, just ignore the pkt
                    syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_ERROR, "%s invalid pkt (pkt_id = %d, pkt_size = %d)\n", cli->client_name, pkt_id, pkt_size);
                }
                base_idx += (pkt_size + 2);
            }
            else
            {
                // not a complete packet
                break;
            }
        }
        if (ret > 0)
        {
            cli->rx_idx = idx - base_idx;
            if (base_idx != 0)
            {               
                memcpy(cli->rx_buf, &cli->rx_buf[base_idx], cli->rx_idx);
            }
        }
    }
    else
    {
        syslog_write(cli->server->syslog_handle, SYSLOG_LEVEL_ERROR,"%s com_fd_rd_callback: read %d bytes, close fd.\n",cli->client_name, ret);
        fd = multiplex_io_get_fd(io_handle);
        
        multiplex_io_handle_del(cli->server->local_server_mio, io_handle);
        close(fd);
        list_remove(&cli->list);
        while (!list_isempty(&cli->ref_list))
        {
            cli_ref = list_first_entry(&cli->ref_list, client_runtime_ref_list_t, ref_list);
            list_remove(&cli_ref->list);
            list_remove(&cli_ref->ref_list);
            free(cli_ref);
        }
        free(cli);
        ret = -1;
    }
    return ret;
}
/*
static int com_fd_wt_callback(multiplex_io_handle_t io_handle, void *param)
{
    int ret = 0;
    printf("com_fd_wt_callback(0x%X, 0x%X)\n", (unsigned int)io_handle, (unsigned int)param);
    return ret;
}
static int com_fd_err_callback(multiplex_io_handle_t io_handle, void *param)
{
    int ret = 0;
    printf("com_fd_err_callback(0x%X, 0x%X)\n", (unsigned int)io_handle, (unsigned int)param);
    return ret;
}
*/
static int listen_fd_rd_callback(multiplex_io_handle_t io_handle, void *param)
{
    int ret = 0;
    int com_fd;
    int len;
    client_runtime_t *cli_runtime;
    multiplex_io_cfg_t mio_cfg;
    local_server_t *server = (local_server_t *)param;
    struct sockaddr_un clt_addr;
    int listen_fd = multiplex_io_get_fd(io_handle);
    if (listen_fd >= 0)
    {
        len=sizeof(clt_addr);
        com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, (socklen_t *)&len);
        if (com_fd >= 0)
        {
            cli_runtime = malloc(sizeof(client_runtime_t));
            if (cli_runtime != NULL)
            {
                cli_runtime->client_name[0] = '\0';
                cli_runtime->rx_idx = 0;
                cli_runtime->server = server;
                list_init(&cli_runtime->list);
                list_init(&cli_runtime->ref_list);
                mio_cfg.fd = com_fd;
                mio_cfg.rd_ind = com_fd_rd_callback;
                mio_cfg.rd_ind_param = cli_runtime;
                mio_cfg.wr_conf = NULL;//com_fd_wt_callback;
                mio_cfg.wr_conf_param = cli_runtime;
                mio_cfg.err_ind = NULL; //com_fd_err_callback;
                cli_runtime->io_handle = multiplex_io_handle_add(cli_runtime->server->local_server_mio, &mio_cfg);
                if (NULL != cli_runtime->io_handle)
                {
                    //printf("listen_fd_rd_callback: new client accepted.\n");
                    //write(com_fd, &com_fd, sizeof(com_fd));
                    list_insert_before(&cli_runtime->server->subscribe_client_list[0], &cli_runtime->list);
                    ret = 0;
                }
                else
                {
                    syslog_write(server->syslog_handle, SYSLOG_LEVEL_ERROR, "listen_fd_rd_callback: multiplex_io_handle_add() fail.\n");
                    free(cli_runtime);
                    close(com_fd);
                    ret = -4;
                }
            }
            else
            {
                syslog_write(server->syslog_handle, SYSLOG_LEVEL_ERROR, "listen_fd_rd_callback: malloc(sizeof(client_runtime_t)) fail.\n");
                close(com_fd);
                ret = -3;
            }
        }
        else
        {
            syslog_write(server->syslog_handle, SYSLOG_LEVEL_ERROR, "listen_fd_rd_callback: com_fd = %d\n", com_fd);
            ret = -2;
        }
    }
    else
    {
        syslog_write(server->syslog_handle, SYSLOG_LEVEL_ERROR, "listen_fd_rd_callback: listen_fd = %d\n", listen_fd);
        ret = -1;
    }
    return ret;
}

local_server_t * local_server_start(const char *addr)
{
    local_server_t * ret = NULL;
    int rv;
    int listen_fd;
    int fd_flags;
    int i;
    struct sockaddr_un srv_addr;
    multiplex_io_cfg_t mio_cfg;
    ret = malloc(sizeof(local_server_t));
    if (ret != NULL)
    {
        ret->syslog_handle = NULL;
        strncpy(ret->file_name, addr, sizeof(ret->file_name)-1);
        ret->local_server_mio = NULL;
        ret->listen_io_handle = NULL;
        for (i=0; i<sizeof(ret->subscribe_client_list)/sizeof(ret->subscribe_client_list[0]); i++)
        {
            list_init(&ret->subscribe_client_list[i]);
        }
        
        unlink(addr);
        //multiplex_io_close(ret->local_server_mio);
        ret->local_server_mio = multiplex_io_open();
        if (ret->local_server_mio != NULL)
        {
            listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
            if (listen_fd >= 0)
            {
                fd_flags = fcntl(listen_fd, F_GETFL, 0);
                if ((-1 != fd_flags) && (-1 != fcntl(listen_fd, F_SETFL, fd_flags | O_NONBLOCK)))
                {
                    srv_addr.sun_family = AF_UNIX;
                    strncpy(srv_addr.sun_path, addr, sizeof(srv_addr.sun_path) - 1);
                    rv = bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
                    if (rv >= 0)
                    {
                        rv = listen(listen_fd, 5);
                        if (rv >= 0)
                        {
                            mio_cfg.fd = listen_fd;
                            mio_cfg.rd_ind = listen_fd_rd_callback;
                            mio_cfg.rd_ind_param = ret;
                            mio_cfg.wr_conf = NULL;
                            mio_cfg.wr_conf_param = NULL;
                            mio_cfg.err_ind = NULL;//listen_fd_err_callback;
                            ret->listen_io_handle = multiplex_io_handle_add(ret->local_server_mio, &mio_cfg);
                            if (NULL == ret->listen_io_handle)
                            {
                                syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: multiplex_io_handle_add() fail.\n");
                                close(listen_fd);
                                multiplex_io_close(ret->local_server_mio);
                                ret->local_server_mio = NULL;
                                free(ret);
                                ret = NULL;
                            }
                            else
                            {
                                //printf("local_server_start(%s) ok.\n", addr);
                            }
                        }
                        else
                        {
                            syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: listen() fail.\n");
                            close(listen_fd);
                            multiplex_io_close(ret->local_server_mio);
                            ret->local_server_mio = NULL;
                            free(ret);
                            ret = NULL;
                        }
                    }
                    else
                    {
                        syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: bind() fail.\n");
                        close(listen_fd);
                        multiplex_io_close(ret->local_server_mio);
                        ret->local_server_mio = NULL;
                        free(ret);
                        ret = NULL;
                    }
                }
                else
                {
                    syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: fcntl() fail.\n");
                    close(listen_fd);
                    multiplex_io_close(ret->local_server_mio);
                    ret->local_server_mio = NULL;
                    free(ret);
                    ret = NULL;
                }
            }
            else
            {
                syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: socket() fail.\n");
                multiplex_io_close(ret->local_server_mio);
                ret->local_server_mio = NULL;
                free(ret);
                ret = NULL;
            }
        }
        else
        {
            syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: multiplex_io_open() fail.\n");
            free(ret);
            ret = NULL;
        }
        if (ret == NULL)
        {
            unlink(addr);
        }
    }
    else
    {
        syslog_write(ret->syslog_handle, SYSLOG_LEVEL_ERROR,"local_server_start: malloc() fail.\n");
    }
    return ret;
}

void local_server_stop(local_server_t * local_server)
{
    client_runtime_t *cli;
    client_runtime_ref_list_t *cli_ref;
    int fd;
    int i;
    if (local_server != NULL)
    {
        unlink(local_server->file_name);
        fd = multiplex_io_get_fd(local_server->listen_io_handle);
        multiplex_io_handle_del(local_server->local_server_mio, local_server->listen_io_handle);
        close(fd);
        while (!list_isempty(&local_server->subscribe_client_list[0]))
        {

            cli = list_entry(local_server->subscribe_client_list[0].next, client_runtime_t, list);
            fd = multiplex_io_get_fd(cli->io_handle);
            list_remove(&cli->list);
            multiplex_io_handle_del(local_server->local_server_mio, cli->io_handle);
            close(fd);
            free(cli);
        }
        for (i=1; i<sizeof(local_server->subscribe_client_list)/sizeof(local_server->subscribe_client_list[0]); i++)
        {
            while (!list_isempty(&local_server->subscribe_client_list[i]))
            {
                cli_ref = list_entry(local_server->subscribe_client_list[i].next, client_runtime_ref_list_t, list);
                list_remove(&cli_ref->list);
                free(cli_ref);
            }
        }
        multiplex_io_close(local_server->local_server_mio);
        local_server->local_server_mio = NULL;
        free(local_server);
    }
}

