/**
 * @brief Export Interfaces of multiplex_io module
 * 
 * @file multiplex_io.h
 * @author Fred
 * @date 2018-07-14
 */
#ifndef MULTIPLEX_IO_H
#define MULTIPLEX_IO_H


/**
 * @brief control options definitions for multiplex_io_handle_ctl
 * 
 */
#define MIO_CTL_OP_GET_CFG (1)
#define MIO_CTL_OP_SET_CFG (2)
#define MIO_CTL_OP_RESET_WRITE_QUEUE (3)
#define MIO_CTL_OP_GET_WRITE_QUEUE_LEN (4)

/**
 * @brief return status definitions
 * 
 */
#define MIO_STS_OK                  (0)
#define MIO_ERR_INVALID_PARAM       (-1)
#define MIO_ERR_GENERAL_ERR         (-2)

#define MIO_E_IND_STS_ERROR        (1)
#define MIO_E_IND_STS_HUNGUP       (2)


/**
 * @brief multiplex_io instance type definition
 *
 */
typedef struct multiplex_runtime_t multiplex_runtime_t;
typedef struct multiplex_handle_t multiplex_handle_t;


typedef multiplex_runtime_t * multiplex_io_t;
typedef multiplex_handle_t * multiplex_io_handle_t;
/**
 * @brief call back function pointer type definition
 * 
 */
typedef int (*io_callback_t)(multiplex_io_handle_t io_handle, void *param);

/**
 * @brief multiplex io configuration type definition
 * 
 */
typedef struct
{
    int fd;
    io_callback_t rd_ind;
    io_callback_t wr_conf;
    io_callback_t err_ind;
    void * rd_ind_param;
    void * wr_conf_param;
} multiplex_io_cfg_t;

multiplex_io_t multiplex_io_open(void);
multiplex_io_handle_t multiplex_io_handle_add(multiplex_io_t handle, multiplex_io_cfg_t *cfg);
int multiplex_io_handle_del(multiplex_io_t handle, multiplex_io_handle_t io);
int multiplex_io_handle_ctl(multiplex_io_t handle, multiplex_io_handle_t io, int op, void * param);
int multiplex_io_close(multiplex_io_t handle);
int multiplex_io_write(multiplex_io_t handle, multiplex_io_handle_t io, void *data, int data_len);
int multiplex_io_read(multiplex_io_t handle, multiplex_io_handle_t io, void *buf, int buf_size);
int multiplex_io_get_fd(multiplex_io_handle_t io);

#endif
