#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<sys/un.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdarg.h>
#include "libsysbus.h"
//#define LIBSYSBUS_USE_MIO
#define LIBSYSBUS_USE_LOCAL_TX_BUF
#ifdef LIBSYSBUS_USE_MIO
#include "multiplex_io.h"
#endif

#define SYSBUS_NAME_SIZE (256)
#define SYSBUS_RX_BUF_SIZE (65540)

struct sysbus_handle_t
{
    char bus_name[SYSBUS_NAME_SIZE];
    char client_name[SYSBUS_NAME_SIZE];
    int fd;
#ifdef LIBSYSBUS_USE_MIO
    multiplex_io_t mio_handle;
    multiplex_io_handle_t mio_io;
    int rx_total_size;
#else
    pthread_t rx_thread;
#endif
    pkt_proc_fn_t pkt_proc_fn;
    void * proc_fn_param;

    unsigned char log_lvl;
    unsigned char rx_buff[SYSBUS_RX_BUF_SIZE];
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
    unsigned char tx_buff[SYSBUS_RX_BUF_SIZE];
    pthread_mutex_t tx_lock;
#endif
} ;

#ifdef LIBSYSBUS_USE_MIO
static int mio_rd_callback(multiplex_io_handle_t io_handle, void *param)
{
    sysbus_handle_t *handle = (sysbus_handle_t*)param;
    int rx_total_size, i;
    int rx_len;
    int ret = -1;
    unsigned short pkt_id, pkt_size;
    if (handle != NULL)
    {
        ret = 0;
        rx_total_size = handle->rx_total_size;
        do
        {
            rx_len = multiplex_io_read(handle->mio_handle, io_handle, &handle->rx_buff[rx_total_size], sizeof(handle->rx_buff) - rx_total_size);
            if (rx_len > 0)
            {
                rx_total_size += rx_len;
                i=0;
                while (i + 3 < rx_total_size)
                {
                    pkt_size = (handle->rx_buff[i] | ((unsigned short)handle->rx_buff[i+1] << 8));
                    pkt_id = (handle->rx_buff[i+2] | ((unsigned short)handle->rx_buff[i+3] << 8));
                    if (i + pkt_size + 2 <= rx_total_size)
                    {
                        if (handle->pkt_proc_fn != NULL)
                        {
                            handle->pkt_proc_fn(pkt_id, &handle->rx_buff[i+4], pkt_size-2, handle->proc_fn_param);
                        }
                        i = i + pkt_size + 2;
                    }
                    else
                    {
                        break;
                    }
                }
                if (i > 0)
                {
                    memcpy(handle->rx_buff, &handle->rx_buff[i], rx_total_size - i);
                    rx_total_size -= i;
                }
            }
        } while (rx_len > 0);
        handle->rx_total_size = rx_total_size;
        if (handle->pkt_proc_fn != NULL)
        {
            handle->pkt_proc_fn(0, NULL, 0, handle->proc_fn_param);
        }
    }
    return ret;
}
#else
static void *sysbus_rx_thread(void *param)
{
    sysbus_handle_t *handle = (sysbus_handle_t*)param;
    int rx_total_size, i;
    int rx_len;
    unsigned short pkt_id, pkt_size;
    syslog_write(handle, SYSLOG_LEVEL_INFO, "rx_len=%d.\n", rx_len);
    if (handle != NULL)
    {
        rx_total_size = 0;
        do
        {
            rx_len = read(handle->fd, &handle->rx_buff[rx_total_size], sizeof(handle->rx_buff) - rx_total_size);
            syslog_write(handle, SYSLOG_LEVEL_INFO, "rx_len=%d.\n", rx_len);
            if (rx_len > 0)
            {
                rx_total_size += rx_len;
                i=0;
                while (i + 3 < rx_total_size)
                {
                    pkt_size = (handle->rx_buff[i] | ((unsigned short)handle->rx_buff[i+1] << 8));
                    pkt_id = (handle->rx_buff[i+2] | ((unsigned short)handle->rx_buff[i+3] << 8));
                    if (i + pkt_size + 2 <= rx_total_size)
                    {
                        if (handle->pkt_proc_fn != NULL)
                        {
                            handle->pkt_proc_fn(pkt_id, &handle->rx_buff[i+4], pkt_size-2, handle->proc_fn_param);
                        }
                        i = i + pkt_size + 2;
                    }
                    else
                    {
                        break;
                    }
                }
                if (i > 0)
                {
                    memcpy(handle->rx_buff, &handle->rx_buff[i], rx_total_size - i);
                    rx_total_size -= i;
                }
            }
        } while (rx_len > 0);
        if (handle->pkt_proc_fn != NULL)
        {
            handle->pkt_proc_fn(0, NULL, 0, handle->proc_fn_param);
        }
    }
    return param;
}
#endif

sysbus_handle_t *sysbus_connect(char *bus_name, char *client_name, pkt_proc_fn_t pkt_proc_callback, void *callback_param)
{
    sysbus_handle_t *ret;
    struct sockaddr_un un;
    int len;
    ret = malloc(sizeof(sysbus_handle_t));
    if (ret != NULL)
    {
        ret->fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (ret->fd >= 0)
        {
            un.sun_family = AF_UNIX;
            strcpy(un.sun_path, bus_name);
            if ((connect(ret->fd, (struct sockaddr *)&un, sizeof(un))) >= 0)
            {
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                if (0 == pthread_mutex_init(&ret->tx_lock, NULL))
#endif
                {
                    strncpy(ret->bus_name, bus_name, sizeof(ret->bus_name));
                    strncpy(&ret->client_name[2], client_name, sizeof(ret->client_name) - 2);
                    ret->pkt_proc_fn = pkt_proc_callback;
                    ret->proc_fn_param = callback_param;
                    ret->log_lvl = SYSLOG_LEVEL_DBG;
#ifdef LIBSYSBUS_USE_MIO
                    multiplex_io_cfg_t mio_cfg;
                    ret->rx_total_size = 0;
                    ret->mio_handle = multiplex_io_open();
                    if (ret->mio_handle == NULL)
                    {
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                        pthread_mutex_destroy(&ret->tx_lock);
#endif
                        close(ret->fd);
                        free(ret);
                        ret = NULL;
                    }
                    else
                    {
                        mio_cfg.fd = ret->fd;
                        mio_cfg.rd_ind = mio_rd_callback,
                        mio_cfg.rd_ind_param = ret;
                        mio_cfg.wr_conf = NULL;
                        mio_cfg.wr_conf_param = NULL;
                        mio_cfg.err_ind = NULL;
                        ret->mio_io = multiplex_io_handle_add(ret->mio_handle, &mio_cfg);
                        if (ret->mio_io == NULL)
                        {
                            multiplex_io_close(ret->mio_handle);
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                            pthread_mutex_destroy(&ret->tx_lock);
#endif
                            close(ret->fd);
                            free(ret);
                            ret = NULL;
                        }
                        else
                        {
                            ret->client_name[0] = 0;
                            ret->client_name[1] = 0;
                            len = 3 + strlen(&ret->client_name[2]);
                            if (len != sysbus_msg_send(ret, 0x0000, (unsigned char *)ret->client_name, len))
                            {
                                multiplex_io_handle_del(ret->mio_handle, ret->mio_io);
                                multiplex_io_close(ret->mio_handle);
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                                pthread_mutex_destroy(&ret->tx_lock);
#endif
                                close(ret->fd);
                                free(ret);
                                ret = NULL;
                            }
                        }
                    }
#else
                    if (0 != pthread_create(&ret->rx_thread, NULL, sysbus_rx_thread, ret))
                    {
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                        pthread_mutex_destroy(&ret->tx_lock);
#endif
                        close(ret->fd);
                        free(ret);
                        ret = NULL;
                    }
                    else
                    {
                        ret->client_name[0] = 0;
                        ret->client_name[1] = 0;
                        len = 3 + strlen(&ret->client_name[2]);
                        if (len != sysbus_msg_send(ret, 0x0000, (unsigned char *)ret->client_name, len))
                        {
                            close(ret->fd);
                            pthread_join(ret->rx_thread, NULL);
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                            pthread_mutex_destroy(&ret->tx_lock);
#endif
                            free(ret);
                            ret = NULL;
                        }
                    }
#endif
                }
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
                else
                {
                    close(ret->fd);
                    free(ret);
                    ret = NULL;
                }
#endif
            }
            else
            {
                close(ret->fd);
                free(ret);
                ret = NULL;
            }
        }
        else
        {
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}
int sysbus_disconnect(sysbus_handle_t *handle)
{
    int ret = -1;
    if (handle != NULL)
    {
        ret = 0;
#ifdef LIBSYSBUS_USE_MIO
        if (handle->mio_handle != NULL)
        {
            if (handle->mio_io != NULL)
            {
                multiplex_io_handle_del(handle->mio_handle, handle->mio_io);
                handle->mio_handle = NULL;
            }
            multiplex_io_close(handle->mio_handle);
            handle->mio_handle = NULL;
        }
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
        pthread_mutex_destroy(&handle->tx_lock);
#endif
        free(handle);
#else
        void *rv;
        pthread_t t = handle->rx_thread;
        close(handle->fd);
        if (pthread_self() != t)
        {
            pthread_cancel(t);
            pthread_join(t, &rv);
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
            pthread_mutex_destroy(&handle->tx_lock);
#endif
            free(handle);
        }
        else
        {
#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
            pthread_mutex_destroy(&handle->tx_lock);
#endif
            free(handle);
            pthread_exit(NULL);
        }
#endif
    }
    return ret;

}
int sysbus_msg_subscribe(sysbus_handle_t *handle, unsigned short *pkt_ids, int pkt_id_num)
{
    int ret = -1;
    int i;
    unsigned char *buf;
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
    unsigned char tx_buf[SYSBUS_RX_BUF_SIZE];
#endif

    if ((handle != NULL) && (handle->fd >= 0) && (pkt_ids != NULL) && (pkt_id_num > 0))
    {
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
        buf = tx_buf;
#else
        buf = handle->tx_buff;
        pthread_mutex_lock(&handle->tx_lock);
#endif
        buf[0] = (unsigned char)(pkt_id_num*2 + 4);
        buf[1] = (unsigned char)((pkt_id_num*2 + 4) >> 8);
        buf[2] = (unsigned char)0x00;
        buf[3] = (unsigned char)0x00;
        buf[4] = (unsigned char)0x01;
        buf[5] = (unsigned char)0x00;        
        for (i=0; i<pkt_id_num; i++)
        {
            buf[6+i*2] = (unsigned char)pkt_ids[i];
            buf[7+i*2] = (unsigned char)(pkt_ids[i]>>8);
        }
#ifdef LIBSYSBUS_USE_MIO
        ret = pkt_id_num*2 + 6;
        if (MIO_STS_OK != multiplex_io_write(handle->mio_handle, handle->mio_io, buf, ret))
        {
            ret = 6;
        }
#else
        ret = write(handle->fd, buf, pkt_id_num*2 + 6);
#endif

#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
        pthread_mutex_unlock(&handle->tx_lock);
#endif
        ret = (ret - 6) / 2;
    }
    return ret;
}
int sysbus_msg_unsubscribe(sysbus_handle_t *handle, unsigned short *pkt_ids, int pkt_id_num)
{
    int ret = -1;
    int i;
    unsigned char *buf;
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
    unsigned char tx_buf[SYSBUS_RX_BUF_SIZE];
#endif
    if ((handle != NULL) && (handle->fd >= 0) && (pkt_ids != NULL) && (pkt_id_num > 0))
    {
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
        buf = tx_buf;
#else
        buf = handle->tx_buff;
        pthread_mutex_lock(&handle->tx_lock);
#endif
        buf[0] = (unsigned char)(pkt_id_num * 2 + 4);
        buf[1] = (unsigned char)((pkt_id_num*2 + 4) >> 8);
        buf[2] = (unsigned char)0x00;
        buf[3] = (unsigned char)0x00;
        buf[4] = (unsigned char)0x02;
        buf[5] = (unsigned char)0x00;        
        for (i=0; i<pkt_id_num; i++)
        {
            buf[6+i*2] = (unsigned char)pkt_ids[i];
            buf[7+i*2] = (unsigned char)(pkt_ids[i]>>8);
        }
#ifdef LIBSYSBUS_USE_MIO
        ret = pkt_id_num*2 + 6;
        if (MIO_STS_OK != multiplex_io_write(handle->mio_handle, handle->mio_io, buf, ret))
        {
            ret = 6;
        }
#else
        ret = write(handle->fd, buf, pkt_id_num*2 + 6);
#endif

#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
        pthread_mutex_unlock(&handle->tx_lock);
#endif
        ret = (ret - 6) / 2;
    }
    return ret;
}
int sysbus_msg_send(sysbus_handle_t *handle, unsigned short pkt_id, unsigned char *data, int data_len)
{
    int ret = -1;
    unsigned char *buf;
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
    unsigned char tx_buf[SYSBUS_RX_BUF_SIZE];
#endif
    if ((handle != NULL) && (handle->fd >= 0) && (data_len + 2 <= 65535) && (data_len + 4 <= SYSBUS_RX_BUF_SIZE) && (data != NULL))
    {
#ifdef LIBSYSBUS_USE_LOCAL_TX_BUF
        buf = tx_buf;
#else
        buf = handle->tx_buff;
        pthread_mutex_lock(&handle->tx_lock);
#endif
        buf[0] = (unsigned char)(data_len + 2);
        buf[1] = (unsigned char)((data_len + 2) >> 8);
        buf[2] = (unsigned char)pkt_id;
        buf[3] = (unsigned char)(pkt_id >> 8);
        memcpy(&buf[4], data, data_len);
#ifdef LIBSYSBUS_USE_MIO
        ret = data_len + 4;
        if (MIO_STS_OK != multiplex_io_write(handle->mio_handle, handle->mio_io, buf, ret))
        {
            ret = 0;
        }
#else
        ret = write(handle->fd, buf, data_len + 4);
#endif

#ifndef LIBSYSBUS_USE_LOCAL_TX_BUF
        pthread_mutex_unlock(&handle->tx_lock);
#endif
        if (ret > 4)
        {
            ret -= 4;
        }  
        else
        {
            ret = 0;
        }
    }
    return ret;
}

char *sysbus_get_client_name(sysbus_handle_t *handle)
{
	char *ret = NULL;
	if (handle != NULL)
	{
		ret = &handle->client_name[2];
	}
	return ret;
}

char *sysbus_get_bus_name(sysbus_handle_t *handle)
{
	char *ret = NULL;
	if (handle != NULL)
	{
		ret = handle->bus_name;
	}
	return ret;
}

unsigned char syslog_set_level(sysbus_handle_t *handle, unsigned char level)
{
    unsigned char ret = -1;
    if (handle != NULL)
    {
        if ((level >= SYSLOG_LEVEL_DBG) && (level <= SYSLOG_LEVEL_FATAL))
        {
            handle->log_lvl = level;
        }
        ret = handle->log_lvl;
    }
    return ret;
}

unsigned char syslog_get_level(sysbus_handle_t *handle)
{
    unsigned char ret = -1;
    if (handle != NULL)
    {
        ret = handle->log_lvl;
    }
    return ret;
}

#if 0

int syslog_write(sysbus_handle_t *handle, unsigned char level, const char *fmt, ...)
{
	int len = 0;
    char txt_buf[1024];
    if ((handle != NULL) && (fmt != NULL))
    {
        if (level >= handle->log_lvl)
        {
            va_list arg_ptr;
            va_start(arg_ptr, fmt);
            txt_buf[0] = level;
            len = 1 + snprintf(&txt_buf[1],sizeof(txt_buf)-1, "%s: ", &handle->client_name[2]);
            len += vsnprintf(&txt_buf[len], sizeof(txt_buf)-len,fmt, arg_ptr);
            txt_buf[len] = '\0';
            va_end(arg_ptr);
            len = sysbus_msg_send(handle, SYS_LOG_MSG, (unsigned char *)txt_buf, len + 1);
        }
    }
    return len;
}
#endif

