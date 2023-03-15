#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>  
#include <sys/un.h>  
#include <syslog.h>

#include "libsysbus.h"
#include "sysbus_msg.h"

#define LOG_BUF_SIZE (1600)

int32_t vcar_sys_trace_dump_api(sysbus_handle_t *sysbus_handle, int32_t level, const char* func, int32_t lineNum, const char *format, ...)
{	
	va_list ap;	
	int32_t ret = 0;
	int32_t len = 0;
	struct timeval tv;
	struct tm *ptm = NULL;
	char line[LOG_BUF_SIZE + 1] = {0};

	if(NULL == format || NULL == sysbus_handle)
    {
		return -1;
	}

    memset(line, 0, sizeof(line));

    // Read system timestamp
    gettimeofday(&tv, NULL);
    ptm = localtime(&tv.tv_sec);

    ret = snprintf(line, sizeof(line)-1, "[%04d-%02d-%02d %02d:%02d:%02d.%06ld]", 
            (ptm->tm_year+1900), ptm->tm_mon+1, ptm->tm_mday, 
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec%1000000);
    len += ret;

    ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", sysbus_get_client_name(sysbus_handle));
    len+=ret;

    switch(level)
    {
        case SYSLOG_LEVEL_DBG:
            ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", "DEBUG");
            len+=ret;
            break;
        case SYSLOG_LEVEL_WARN:
            ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", "WARN");
            len+=ret;
            break;
        case SYSLOG_LEVEL_ERROR:
            ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", "ERROR");
            len+=ret;
            break;
        default:
            ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", "DEBUG");
            len+=ret;
            break;
    }

    if(func){
        ret = snprintf(line+len, sizeof(line)-1-len, "[%s]", func);
        len+=ret;
    }

    if(lineNum > 0){
        ret = snprintf(line+len, sizeof(line)-1-len, "[%d]", lineNum);
        len+=ret;
    }

	va_start(ap,format);
	vsnprintf(line+len, sizeof(line)-1-ret, format,ap);
	va_end(ap);

    ret = sysbus_msg_send(sysbus_handle, SYSBUS_SYS_LOG_ID, (unsigned char *)line, strlen(line));
	
	fprintf(stdout, "%s",line);
	fflush(stdout);

	return 0;
}


