#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include "multiplex_io.h"
#include "list.h"

//#define MIO_USE_BUF_LIST
#define MIO_RB_BUF_SIZE (1024*1024) /* shall be 2^n */

/*
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} cond_t;
*/

typedef struct buf_head_tag
{
    int buf_size;
    int buf_idx;
    unsigned char * data;
    list_t list;
} buf_head_t;

typedef struct 
{
    unsigned int r_idx;
    unsigned int w_idx;
    unsigned char data[MIO_RB_BUF_SIZE];
} mio_rb_t;

struct multiplex_handle_t
{
    struct epoll_event epevt;
    int fd;
    pthread_mutex_t mutex;
    io_callback_t r_ind;
    io_callback_t w_conf;
    io_callback_t e_ind;
#ifdef MIO_USE_BUF_LIST
    list_t buf_list;
#else
    mio_rb_t w_buf;
#endif
    void * r_param;
    void * w_param;
    list_t list;
}; 

struct multiplex_runtime_t
{
    int epfd;
    pthread_t thread;
    pthread_mutex_t mutex;
    int thread_running;
    list_t io_handle_list;
    list_t del_handle_list;
};
/*
void cond_init(cond_t *cond)
{
    if (cond != NULL)
    {
        pthread_mutex_init(&cond->mutex, NULL);
        pthread_cond_init(&cond->cond, NULL);
    }
}

int cond_wait(cond_t *cond, int timeout)
{
    int ret = -1;
    struct timespec tm;
    if (cond != NULL)
    {
        ret = -2;
        if (0 == pthread_mutex_lock(&cond->mutex))
        {
            clock_gettime(CLOCK_REALTIME, &tm);
            //pthread_cond_timedwait(&cond->cond, &cond->mutex, )
        }
    }
    return ret;
}
*/

static int multiplex_io_errorno_to_sts(int err_no)
{
    int ret;
    ret = err_no * (-1) + MIO_ERR_GENERAL_ERR;
    if (ret >= MIO_ERR_GENERAL_ERR)
    {
        ret = MIO_ERR_GENERAL_ERR;
    }
    return ret;
}

static void multiplex_io_handle_list_add(multiplex_runtime_t *handle, multiplex_handle_t *io)
{
    if (list_isempty(&io->list))
    {
        pthread_mutex_lock(&handle->mutex);
        list_insert_before(&handle->io_handle_list, &io->list);
        pthread_mutex_unlock(&handle->mutex);
    }
}

static void multiplex_io_buf_list_free(multiplex_handle_t *io)
{
#ifdef MIO_USE_BUF_LIST
    buf_head_t *buf;
    list_t *n;
    while(!list_isempty(&io->buf_list))
    {
        n = io->buf_list.next;
        list_remove(n);
        buf = list_entry(n, buf_head_t, list);
        free(buf);
    }
#else
    io->w_buf.r_idx = io->w_buf.w_idx;
#endif
}
static void multiplex_io_handle_list_free(multiplex_handle_t *io)
{
    list_remove(&io->list);
    multiplex_io_buf_list_free(io);
    free(io);
}

#ifndef MIO_USE_BUF_LIST
/*
static unsigned int multiplex_io_rb_get_free_size(mio_rb_t *rb)
{
    unsigned int r_idx, w_idx, size, ret;
    r_idx = rb->r_idx;
    w_idx = rb->w_idx;
    size = sizeof(rb->data);
    if (r_idx <= w_idx)
    {
        ret = size - 1 - (w_idx - r_idx);
    }
    else
    {
        ret = r_idx - w_idx - 1;

    }
    return ret;
}
*/

static unsigned int multiplex_io_rb_write_data(mio_rb_t *rb, unsigned char *data, unsigned int len)
{
    unsigned int r_idx, w_idx, size, free_size, ret, tmp;
    size = sizeof(rb->data);
    r_idx = rb->r_idx;
    w_idx = rb->w_idx;
    if (r_idx <= w_idx)
    {
        free_size = size - 1 - (w_idx - r_idx);
        tmp = size - w_idx;
    }
    else
    {
        free_size = r_idx - w_idx - 1;
        tmp = free_size;
    }
    if (free_size >= len)
    {
        if (tmp >= len)
        {
            memcpy(&rb->data[w_idx], data, len);
            w_idx += len;
        }
        else
        {
            memcpy(&rb->data[w_idx], data, tmp);
            memcpy(&rb->data[0], &data[tmp], len-tmp);
            w_idx = len - tmp;
        }
        rb->w_idx = w_idx;
        ret = len;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

static unsigned int multiplex_io_rb_get_data_size(mio_rb_t *rb)
{
    unsigned int r_idx, w_idx, size, ret;
    r_idx = rb->r_idx;
    w_idx = rb->w_idx;
    size = sizeof(rb->data);
    if (r_idx <= w_idx)
    {
        ret = w_idx - r_idx;
    }
    else
    {
        ret = size - r_idx + w_idx;
    }
    return ret;
}
#endif

static void * multiplex_io_thread(void *param)
{
    multiplex_runtime_t *handle = (multiplex_runtime_t *)param;
    //int ret = MIO_ERR_INVALID_PARAM;
    int nfds;
    int i;
    multiplex_handle_t *h;
    struct epoll_event events[20];
    uint32_t tmp_event;
#ifdef MIO_USE_BUF_LIST
    buf_head_t *buf;
#else
    unsigned int r_idx;
    unsigned int w_idx;
#endif
    if (handle != NULL)
    {
        while (handle->thread_running)
        {
            nfds = epoll_wait(handle->epfd, events, sizeof(events)/sizeof(events[0]), 500);
            if (nfds >= 0)
            {
                for(i = 0; i < nfds; i++)
                {
                    h = (multiplex_handle_t *)events[i].data.ptr;
                    if (list_contains_node(&handle->io_handle_list, &h->list))
                    {
                        tmp_event = h->epevt.events;
                        if (events[i].events & EPOLLIN)
                        {
                            if (h->r_ind != NULL)
                            {
                                h->r_ind(h, h->r_param);
                                tmp_event |= (EPOLLIN);
                            }
                            else
                            {
                                tmp_event &= (~EPOLLIN);
                            }
                        }
                        if (events[i].events & EPOLLOUT)
                        {
                        #ifdef MIO_USE_BUF_LIST
                            if (!list_isempty(&h->buf_list))
                            {
                                buf = list_entry(h->buf_list.next, buf_head_t, list);
                                buf->buf_idx += write(h->fd, &buf->data[buf->buf_idx], buf->buf_size - buf->buf_idx);
                                if (buf->buf_idx >= buf->buf_size)
                                {
                                    list_remove(h->buf_list.next);
                                    if (h->w_conf != NULL)
                                    {
                                        h->w_conf(h, h->w_param);
                                    }
                                    free(buf);
                                    if (list_isempty(&h->buf_list))
                                    {
                                        tmp_event &= (~EPOLLOUT);
                                    }
                                    else
                                    {
                                        tmp_event |= EPOLLOUT;
                                    }
                                }
                            }
                            else
                            {
                                tmp_event &= (~EPOLLOUT);
                            }
                        #else
                            r_idx = h->w_buf.r_idx;
                            pthread_mutex_lock(&h->mutex);
                            w_idx = h->w_buf.w_idx;
                            pthread_mutex_unlock(&h->mutex);
                            if (r_idx != w_idx)
                            { 
                                if (w_idx > r_idx)
                                {
                                    r_idx += write(h->fd, &h->w_buf.data[r_idx], w_idx - r_idx);
                                }
                                else
                                {
                                    r_idx += write(h->fd, &h->w_buf.data[r_idx], sizeof(h->w_buf.data) - r_idx);
                                    r_idx = r_idx % sizeof(h->w_buf.data);
                                    if (r_idx == 0)
                                    {
                                        r_idx += write(h->fd, &h->w_buf.data[r_idx], w_idx);
                                    }
                                }
                                h->w_buf.r_idx = r_idx;
                                if (r_idx == w_idx)
                                {
                                    if (h->w_conf != NULL)
                                    {
                                        //printf("multiplex_io_thread: w_conf() called. (r_idx, w_idx) = (%d, %d)\n", r_idx, w_idx);
                                        h->w_conf(h, h->w_param);
                                    }
                                    tmp_event &= (~EPOLLOUT);
                                }
                                else
                                {
                                    tmp_event |= EPOLLOUT;
                                }
                            }
                            else
                            {
                                tmp_event &= (~EPOLLOUT);
                            }
                        #endif
                        }
                        pthread_mutex_lock(&h->mutex);
                        r_idx = h->w_buf.r_idx;
                        w_idx = h->w_buf.w_idx;
                        if (r_idx != w_idx)
                        {
                            tmp_event |= EPOLLOUT;
                        }
                        if (tmp_event != h->epevt.events)
                        {
                            h->epevt.events = tmp_event;
                            epoll_ctl(handle->epfd, EPOLL_CTL_MOD, h->fd, &h->epevt);
                        }
                        pthread_mutex_unlock(&h->mutex);
                        tmp_event = 0;
                        if (events[i].events & (EPOLLRDHUP|EPOLLHUP))
                        {
                            tmp_event |= MIO_E_IND_STS_HUNGUP;
                        }
                        if (events[i].events & EPOLLERR)
                        {
                            tmp_event |= MIO_E_IND_STS_ERROR;
                        }
                        if ((tmp_event) && (h->e_ind != NULL))
                        {
                            h->e_ind(h, (void *)&tmp_event);
                        }
                    }
                }
            }
            else 
            {
                //ret = multiplex_io_errorno_to_sts(errno);
                break;
            }
            while (!list_isempty(&handle->del_handle_list))
            {
                pthread_mutex_lock(&handle->mutex);
                h = list_entry(handle->del_handle_list.next, multiplex_handle_t, list);
                list_remove(handle->del_handle_list.next);
                pthread_mutex_unlock(&handle->mutex);
                epoll_ctl(handle->epfd, EPOLL_CTL_DEL, h->fd, &h->epevt);
                multiplex_io_handle_list_free(h);
            }
        }
    }
    pthread_detach(pthread_self());
    return param;
}

multiplex_io_t multiplex_io_open(void)
{
    multiplex_runtime_t *ret = NULL;
    int epfd;
    epfd = epoll_create1(0);
    if (epfd >= 0)
    {
        ret = (multiplex_runtime_t *)malloc(sizeof(multiplex_runtime_t));
        if (ret != NULL)
        {
            ret->epfd = epfd;
            pthread_mutex_init(&ret->mutex, NULL);
            list_init(&ret->del_handle_list);
            list_init(&ret->io_handle_list);
            ret->thread_running = 1;
            pthread_create(&ret->thread, NULL, multiplex_io_thread, ret);
        }
        else
        {
            close(epfd);
        }
    }
    return (ret);
}

multiplex_io_handle_t multiplex_io_handle_add(multiplex_io_t handle, multiplex_io_cfg_t *cfg)
{
    //multiplex_runtime_t *handle = multiplex_io;
    int fd_flags;
    multiplex_handle_t *io = NULL;
    if ((handle != NULL) && (handle->epfd >= 0) && (cfg != NULL))
    {
        io = (multiplex_handle_t *)malloc(sizeof(multiplex_handle_t));
        if (io != NULL)
        {
            fd_flags = fcntl(cfg->fd,F_GETFL, 0);
            if ((-1 != fd_flags) && (-1 != fcntl(cfg->fd,F_SETFL,fd_flags|O_NONBLOCK)))
            {
                pthread_mutex_init(&io->mutex, NULL);
                io->epevt.events = EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
                io->epevt.data.ptr = io;
                io->r_ind = cfg->rd_ind;
                io->w_conf = cfg->wr_conf;
                io->e_ind = cfg->err_ind;
                io->w_param = cfg->wr_conf_param;
                io->r_param = cfg->rd_ind_param;
            #ifdef MIO_USE_BUF_LIST
                list_init(&io->buf_list);
            #else
                io->w_buf.r_idx = io->w_buf.w_idx = 0;
            #endif
                list_init(&io->list);
                io->fd = cfg->fd;
                if (0 != epoll_ctl(handle->epfd, EPOLL_CTL_ADD, cfg->fd, &io->epevt))
                {
                    free(io);
                    io = NULL;
                }
                else
                {
                    multiplex_io_handle_list_add(handle, io);
                }
            }
            else
            {
                free(io);
                io = NULL;
            }
        }
        
    }
    return io;
}

int multiplex_io_handle_del(multiplex_io_t handle, multiplex_io_handle_t io)
{
    //multiplex_runtime_t *handle = multiplex_io;
    //multiplex_handle_t *io = io_handle;
    int ret = MIO_ERR_INVALID_PARAM;
    if ((handle != NULL) && (io != NULL) && list_contains_node(&handle->io_handle_list, &io->list))
    {
        ret = MIO_STS_OK;
        pthread_mutex_lock(&handle->mutex);
        list_remove(&io->list);
        list_insert_before(&handle->del_handle_list, &io->list);
        pthread_mutex_unlock(&handle->mutex);
    }
    return ret;
}
int multiplex_io_handle_ctl(multiplex_io_t handle, multiplex_io_handle_t io, int op, void * param)
{
    //multiplex_runtime_t *handle = multiplex_io;
    //multiplex_handle_t *io = io_handle;
    int ret = MIO_ERR_INVALID_PARAM;
    int n;
    multiplex_io_cfg_t *cfg;
#ifdef MIO_USE_BUF_LIST
    list_t *buf_list_node;
#endif
    if ((handle != NULL) && (io != NULL) && list_contains_node(&handle->io_handle_list, &io->list))
    {
        ret = MIO_STS_OK;
        switch (op)
        {
        case MIO_CTL_OP_GET_CFG:
            if (param != NULL)
            {
                cfg = (multiplex_io_cfg_t *)param;
                cfg->fd = io->fd;
                cfg->rd_ind = io->r_ind;
                cfg->wr_conf = io->w_conf;
                cfg->err_ind = io->e_ind;
                cfg->rd_ind_param = io->r_param;
                cfg->wr_conf_param = io->w_param;
            }
            else
            {
                ret = MIO_ERR_INVALID_PARAM;
            }
            break;
        case MIO_CTL_OP_SET_CFG:
            if (param != NULL)
            {
                cfg = (multiplex_io_cfg_t *)param;
                //pthread_mutex_lock(&handle->mutex);
                io->fd = cfg->fd;
                io->r_ind = cfg->rd_ind;
                io->w_conf = cfg->wr_conf;
                io->e_ind = cfg->err_ind;
                io->r_param = cfg->rd_ind_param;
                io->w_param = cfg->wr_conf_param;
                //pthread_mutex_unlock(&handle->mutex);
            }
            else
            {
                ret = MIO_ERR_INVALID_PARAM;
            }
            break;
        case MIO_CTL_OP_GET_WRITE_QUEUE_LEN:
            if (param != NULL)
            {
            #ifdef MIO_USE_BUF_LIST
                n = 0;
                buf_list_node = io->buf_list.next;
                while (buf_list_node != &io->buf_list)
                {
                    ++n;
                    buf_list_node = buf_list_node->next;
                }
            #else
                n = multiplex_io_rb_get_data_size(&io->w_buf);
            #endif
                *((int *)param) = n;
            }
            else
            {
                ret = MIO_ERR_INVALID_PARAM;
            }
            break;
        case MIO_CTL_OP_RESET_WRITE_QUEUE:
            multiplex_io_buf_list_free(io);
            break;
        default:
            ret = MIO_ERR_INVALID_PARAM;
            break;
        }
    }
    return ret;
}
int multiplex_io_close(multiplex_io_t handle)
{
    int ret = MIO_ERR_INVALID_PARAM;
    multiplex_handle_t *h;
    void *status;
    //multiplex_runtime_t *handle = multiplex_io;
    if (handle != NULL)
    {
        handle->thread_running = 0;
        pthread_join(handle->thread, &status);
        ret = MIO_STS_OK;
        pthread_mutex_lock(&handle->mutex);
        while (!list_isempty(&handle->del_handle_list))
        {
            h = list_entry(handle->del_handle_list.next, multiplex_handle_t, list);
            list_remove(handle->del_handle_list.next);
            epoll_ctl(handle->epfd, EPOLL_CTL_DEL, h->fd, &h->epevt);
            multiplex_io_handle_list_free(h);
        }
        while (!list_isempty(&handle->io_handle_list))
        {
            h = list_entry(handle->io_handle_list.next, multiplex_handle_t, list);
            list_remove(handle->io_handle_list.next);
            epoll_ctl(handle->epfd, EPOLL_CTL_DEL, h->fd, &h->epevt);
            multiplex_io_handle_list_free(h);
        }
        close(handle->epfd);
        pthread_mutex_unlock(&handle->mutex);
        pthread_mutex_destroy(&handle->mutex);
        free(handle);
    }
    return ret;
}

int multiplex_io_write(multiplex_io_t handle, multiplex_io_handle_t io, void *data, int data_len)
{
    int ret = MIO_ERR_INVALID_PARAM;
#ifdef MIO_USE_BUF_LIST
    buf_head_t *buf;
#endif
    //multiplex_runtime_t *handle = multiplex_io;
    //multiplex_handle_t *io = io_handle;
    if ((handle != NULL) && (io != NULL) && list_contains_node(&handle->io_handle_list, &io->list) && (data != NULL) && (data_len > 0))
    {
        if (io->fd >= 0)
        {
        #ifdef MIO_USE_BUF_LIST
            buf = (buf_head_t *)malloc(sizeof(buf_head_t) + data_len);
            if (buf != NULL)
            {
                buf->buf_idx = 0;
                buf->buf_size = data_len;
                list_init(&buf->list);
                buf->data = (unsigned char *)((unsigned char *)buf + sizeof(buf_head_t));
                memcpy(buf->data, data, data_len);
                pthread_mutex_lock(&handle->mutex);
                list_insert_before(&io->buf_list, &buf->list);
                io->epevt.events |= EPOLLOUT;
                pthread_mutex_unlock(&handle->mutex);
                if (0 == epoll_ctl(handle->epfd, EPOLL_CTL_MOD, io->fd, &io->epevt))
                {
                    ret = MIO_STS_OK;
                }
                else
                {
                    ret = multiplex_io_errorno_to_sts(errno);
                }
                
            }
            else
            {
                ret = multiplex_io_errorno_to_sts(ENOMEM);
            }
        #else
            pthread_mutex_lock(&io->mutex);
            ret = multiplex_io_rb_write_data(&io->w_buf, data, data_len);
            if (ret == data_len)
            {
                io->epevt.events |= EPOLLOUT;
                if (0 == epoll_ctl(handle->epfd, EPOLL_CTL_MOD, io->fd, &io->epevt))
                {
                    ret = MIO_STS_OK;
                }
                else
                {
                    ret = multiplex_io_errorno_to_sts(errno);
                }
            }
            else
            {
                ret = multiplex_io_errorno_to_sts(ENOMEM);
            }
            pthread_mutex_unlock(&io->mutex);
        #endif
        }
        else
        {
            ret = multiplex_io_errorno_to_sts(EBADF);
        }
    }
    return ret;
}

int multiplex_io_get_fd(multiplex_io_handle_t io)
{
    int ret = -1;
    //multiplex_handle_t *io = io_handle;
    if (io != NULL)
    {
        ret = io->fd;
    }
    return ret;
}
int multiplex_io_read(multiplex_io_t handle, multiplex_io_handle_t io, void *buf, int buf_size)
{
    int ret = MIO_ERR_INVALID_PARAM;
    //multiplex_runtime_t *handle = multiplex_io;
    //multiplex_handle_t *io = io_handle;
    if ((handle != NULL) && (io != NULL) && list_contains_node(&handle->io_handle_list, &io->list) && (buf != NULL) && (buf_size > 0))
    {
        if (io->fd >= 0)
        {
            ret = read(io->fd, buf, buf_size);
	        if (ret < 0)
            {
                ret = multiplex_io_errorno_to_sts(errno);
            }
        }
        else
        {
            ret = multiplex_io_errorno_to_sts(EBADF);
        }
    }
    return ret; 
}
