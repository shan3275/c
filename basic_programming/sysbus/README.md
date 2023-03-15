# Reference Manual - sysbus

## 1. Overview

sysbus is a component for inter-process communication. In its communication model, one or more processes can connect to sysbus and subscribe one or more message by specified packet_id in sysbus. Each process connected to sysbus can also send (publish) messages in one or more packet_id. The messages are routed to the processes which subscribe them by sysbusd. One process can subscribe / publish one or more message(s).

sysbus contains following components:
* _sysbusd_: executable file, sysbus service daemon, provide the sysbus and syslog services.
* _libsysbus.so_: share object file (dynamic linking library), library for access sysbus services
* _libsysbus.h_: c header file, sysbus API declarations for developer.

syslog is build-in service for system logging provided by sysbus. This service is optional activated by prompt of sysbusd.

## 2. Usage
Before using sysbus, you should start _sysbusd_ as a background (daemon) process by following prompt in your system.

```
sysbusd sysbus_file_path [log_dir_path]
```

* _sysbusd_ is the the executable file name of sysbus service daemon.

* _sysbus_file_path_ is the sysbus file name which shall be created by _sysbusd_ and accessed by other process as the access point of sysbus.

* _log_dir_path_ is the log file directory path in which log file(s) created by _syslog_ service shall be put. _log_dir_path_ argument is optional, if it is not specified, _syslog_ service will not be activated. If it is specified, it shall be an existed directory path, otherwise, _syslog_ service will be fail.

## 3. API Reference
### 3.1 *sysbus_connect*
Connect to sysbus.
Parameter|Type|Description
--|--|--
bus_name|char*|sysbus file path and name, shall be same as the first argument of _sysbusd_.
client_name|char*|the name of connected process
pkt_proc_callback|pkt_proc_fn_t|rx message callback function pointer, after process received its subscribed message, this function shall be called. If sysbus connection has been terminated by remote side (as runtime exceptions), this function shall also be called with NULL as data and 0 as data_len.
callback_param|void*|user parameter of rx message callback function

Return Type|Description
--|--
sysbus_handle_t*| sysbus handle, NULL if fail.

### 3.2 *sysbus_disconnect*
Disconnect to sysbus and free internal resources.
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle returned by function sysbus_connect()

Return Type|Description
--|--
int|Status code, zero for success non-zero for errors occured

### 3.3 *sysbus_msg_subscribe*
Subscribe message(s) by specified packet id(s).
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle
pkt_ids|unsigned short*|pointer to subscribed packet id(s) array
pkt_id_num|int|number of subscribed packet

Return Type|Description
--|--
int|number of subscribed packet(s)

### 3.4 *sysbus_msg_unsubscribe*
Unsubscribe message(s) by specified packet id(s).
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle
pkt_ids|unsigned short*|pointer to unsubscribed packet id(s) array
pkt_id_num|int|number of unsubscribed packet

Return Type|Description
--|--
int|number of unsubscribed packet(s)

### 3.5 *sysbus_msg_send*
Send (publish) message by specified packet id and data.
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle
pkt_id|unsigned short|send packet id
data|unsigned char *|pointer to packet data
data_len|int|packet data length

Return Type|Description
--|--
int|Actual send data size, if success, return valud shall be equal to data_len

### 3.6 *syslog_set_level*
Set syslog level.
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle
level|unsigned char|syslog level to set, refer to _SYSLOG_LEVEL_

Return Type|Description
--|--
unsigned char|current syslog level
### 3.7 *syslog_get_level*
Get current syslog level.
Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle

Return Type|Description
--|--
unsigned char|current syslog level
### 3.8 *syslog_write*
Write syslog. This function send the message to syslog service, and if current syslog level is not higher than requested level and syslog service is activaed, then the syslog string will be record in log file.

Parameter|Type|Description
--|--|--
handle|sysbus_handle_t*|sysbus handle
level|unsigned char|requested log level, refer to _SYSLOG_LEVEL_
fmt|char *|log text format string
...|va list|fmt string parameters

Return Type|Description
--|--
int|send log message length.

### 3.9 *pkt_proc_fn_t*
Rx message callback function pointer type.
Parameter|Type|Description
--|--|--
pkt_id|unsigned short|Rx message packet id
data|unsigned char*|pointer to rx message data
data_len|int|rx message data length
param|void*|user defined parameter

### 3.10 *SYSLOG_LEVEL*
enum of syslog level
enum|Value|Description
--|--|--
SYSLOG_LEVEL_DBG|0|Debug message log
SYSLOG_LEVEL_INFO|1|Information message log
SYSLOG_LEVEL_WARN|2|Warning message log
SYSLOG_LEVEL_ERROR|3|Error message log
SYSLOG_LEVEL_FATAL|4|Fatal message log
