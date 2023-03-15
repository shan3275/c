#ifndef LIB_SYSBUS_H
#define LIB_SYSBUS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

enum SYSBUS_PKT_ID
{
    SYS_LOG_MSG = 1,
};

enum SYSLOG_LEVEL
{
    SYSLOG_LEVEL_DBG = 0,
    SYSLOG_LEVEL_INFO,
    SYSLOG_LEVEL_WARN,
    SYSLOG_LEVEL_ERROR,
    SYSLOG_LEVEL_FATAL
};

typedef void (*pkt_proc_fn_t)(unsigned short pkt_id, unsigned char *data, int data_len, void *param);

typedef struct sysbus_handle_t sysbus_handle_t;

sysbus_handle_t *sysbus_connect(char *bus_name, char *client_name, pkt_proc_fn_t pkt_proc_callback, void *callback_param);
int sysbus_disconnect(sysbus_handle_t *handle);
int sysbus_msg_subscribe(sysbus_handle_t *handle, unsigned short *pkt_ids, int pkt_id_num);
int sysbus_msg_unsubscribe(sysbus_handle_t *handle, unsigned short *pkt_ids, int pkt_is_num);
int sysbus_msg_send(sysbus_handle_t *handle, unsigned short pkt_id, unsigned char *data, int data_len);

unsigned char syslog_set_level(sysbus_handle_t *handle, unsigned char level);
unsigned char syslog_get_level(sysbus_handle_t *handle);
char *sysbus_get_bus_name(sysbus_handle_t *handle);
char *sysbus_get_client_name(sysbus_handle_t *handle);

int32_t vcar_sys_trace_dump_api(sysbus_handle_t *sysbus_handle, int32_t level, const char* func, int32_t lineNum, const char *format, ...);

#define syslog_write(sysbus_handle, level, fmt, args...) vcar_sys_trace_dump_api(sysbus_handle, level, __func__, __LINE__, fmt, ##args)
#ifdef __cplusplus
}
#endif
#endif
