#ifndef SYSBUS_MSG_H
#define SYSBUS_MSG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

#define SYSBUS_VCI_REQ_ID (0x0010)
#define SYSBUS_VCI_STS_ID (0x0011)
#define SYSBUS_MDF_DAT_ID (0x0012)
#define SYSBUS_BMR_DAT_ID (0x0013)
#define SYSBUS_MDF_REC_CMD_ID (0x0014)
#define SYSBUS_MDF_REC_STS_ID (0x0015)
#define SYSBUS_BMR_REC_CMD_ID (0x0016)
#define SYSBUS_BMR_REC_STS_ID (0x0017)
#define SYSBUS_VIDEO_REC_CMD_ID (0x0018)
#define SYSBUS_VIDEO_REC_STS_ID (0x0019)
#define SYSBUS_AUDIO_REC_CMD_ID (0x001A)
#define SYSBUS_AUDIO_REC_STS_ID (0x001B)
#define SYSBUS_POST_REC_STS_ID (0x001C)
#define SYSBUS_GPS_STS_ID (0x001D)
#define SYSBUS_SYS_LOG_ID (0x001E)
#define SYSBUS_MDF_CYCLE_ID (0x001F)
#define SYSBUS_MDF_BASE_SEG_ID (0x0020)
#define SYSBUS_BMR_BASE_SEG_ID (0x0021)
#define SYSBUS_ETH_REC_CMD_ID (0x0022)   
#define SYSBUS_ETH_REC_STS_ID (0x0023)
#define SYSBUS_CANTP_REQ_ID (0x0024)
#define SYSBUS_CANTP_RESP_ID (0x0025)
#define SYSBUS_VIDEO_RESET_ID (0x0026)
#define SYSBUS_DOWNLOAD_FILE_ID (0x0027)
#define SYSBUS_VCI_RESP_ID (0x0028)
#define SYSBUS_TRG_REC_CMD_ID (0x0029)
#define SYSBUS_TRG_REC_NOTICE_ID (0x002a)
#define SYSBUS_TRIGGLE_STS_ID (0x0030)

//Record Status Report
#define SYSBUS_BMR_REC_ALIVE_STS_ID  (0x0030)
#define SYSBUS_MDF_REC_ALIVE_STS_ID  (0x0031)
#define SYSBUS_VIDEO_REC_ALIVE_STS_ID  (0x0032)
#define SYSBUS_REC_COPY_ID (0x0033)
#define SYSBUS_COND_TRIG_ID (0x0034)

#define SYSBUS_DORMANCY_CMD_ID (0x0040)
#define SYSBUS_DORMANCY_RESP_ID (0x0041)
#define SYSBUS_DORMANCY_REQ_ID (0x0042)
#define SYSBUS_VCICARD_LOST_ID (0x0043)
#define SYSBUS_PCAP_REC_CMD_ID (0x0044)
#define SYSBUS_PCAP_REC_STS_ID (0x0045)
#define SYSBUS_CHANGE_XCP_PID_REQ_ID (0x0046)
#define SYSBUS_CHANGE_XCP_PID_RESP_ID (0x0047)

#define SYSBUS_LED_CTL_ID (0x0050)


#define SYSBUS_UDS_INIT_REQ (0x0052)
#define SYSBUS_UDS_INIT_RES (0x0053)
#define SYSBUS_UDS_MSG_REQ (0x0054)
#define SYSBUS_UDS_MSG_RES (0x0055)
#define SYSBUS_UDS_CLOSE_REQ (0x0056)
#define SYSBUS_UDS_CLOSE_RES (0x0057)
#define SYSBUS_SCRIPT_CTL_CMD_ID (0x0058)

#define SYSBUS_BLF_REC_STS_ID   (0x0059)    // blf sts msg
#define SYSBUS_BLF_BASE_SEG_ID  (0x0060)    // blf base_seg msg
#define SYSBUS_BLF_REC_CMD_ID   (0x0061)    // blf rec cmd

#define SYSBUS_POST_VIDEO_CMD_ID (0x0062)   // rtmp polling id

#define SYSBUS_FILE_TYPE_DAT (1)
#define SYSBUS_FILE_TYPE_BMR (2)
#define SYSBUS_FILE_TYPE_AVI (3)
#define SYSBUS_FILE_TYPE_AAC (4)
#define SYSBUS_FILE_TYPE_PCAP (5)
#define SYSBUS_FILE_TYPE_MF4 (6)
#define SYSBUS_FILE_TYPE_BLF (7)

#define SYSBUS_FILE_SUB_TYPE_DEF (0)
#define SYSBUS_FILE_SUB_TYPE_TRG (1)

/*rtmp web 推流使用*/
#define SYSBUS_VIDEO_CHANNEL_INFO_REQ (0x0070)
#define SYSBUS_VIDEO_CHANNEL_INFO_RES (0x0071)
#define SYSBUS_VIDEO_RTMP_PUSH_REQ (0x0072)
#define SYSBUS_VIDEO_RTMP_PUSH_RES (0x0073)

#define SYSBUS_FAULT_CODE_MSG_ID (0x0063)   //recorder event status report 

/*post 消息结构*/
#define MAX_DEV_NAME 16
typedef struct
{
    uint8_t cmd;                 // 0 - start send data; 1 - stop send data;
    char dev_name[MAX_DEV_NAME]; //eth_rec, the dev_name is lidar1 lidar2 radar and vision, ignore the dev_index
    int32_t dev_index;           //if the dev name is can and camera, it's means channel number
    char post_ip[17];            //send to ip addr, udp packet
    int32_t post_port;             //send to port, udp packet
    int32_t sample_rate;
    int32_t resolution;
    char req_num[33];
} sysbus_post_data_t;

#pragma  pack(1)
typedef struct 
{
    uint8_t cmd;        // 0 - read; 1 - write; 2 - control;
    uint8_t protocol;   // 0 - VCI_Dev; 1 - CAN; 2 - LIN; 3 - FlexRay; 4 - DI; 5 - DO; 6 - AI
    uint8_t channel;
    uint8_t param;
    uint8_t data[0];    // variable length
} sysbus_vci_req_msg_t;
#pragma pack()

#pragma  pack(1)
typedef struct 
{
    uint8_t sts;        // 0 - read; 1 - write; 2 - control; 255 - broadcast;
    uint8_t protocol;   // 0 - VCI_Dev; 1 - CAN; 2 - LIN; 3 - FlexRay; 4 - DI; 5 - DO; 6 - AI; 255 - broadcast
    uint8_t channel;
    uint8_t param;
    uint64_t timestamp;
    uint8_t data[0];    // variable length
} sysbus_vci_sts_msg_t;
#pragma pack()

#pragma  pack(1)
typedef struct
{
    uint16_t rec_id;
    uint8_t mdf_data[0];    // variable length
} sysbus_mdf_dat_msg_t;
#pragma  pack()

#pragma  pack(1)
typedef struct 
{
    uint16_t dlc;            // data length in byte
    uint8_t channel;        // bit0~5: 1 based channel number; bit 6~7: protocol 00b - CAN, 10b - LIN, 01b - FlexRay, 11b - system 
    uint8_t reserved;
    uint64_t timestamp;     // timestamp in us
    uint32_t id;
    uint8_t data[0];        // variable length
} sysbus_bmr_dat_msg_t;
#pragma  pack()

#pragma  pack(1)
typedef struct 
{
    uint8_t cmd;                         // 0 - start record; 1 - stop record; 2 - config video
    uint8_t MeasureType;                 // 0 - Msg timeout; 1 - Sig Value Eval; 2 - Bus Event; 3 - Bus load; 4 - key trigger 5 - default trigger
    uint8_t seg_mode;                    // 0 - size based segment; 1 - time based segment. if cmd is 2, ignore
    uint32_t seg_value;                  // if size based, unit is MB, if time based, unit is min. if cmd is 2, ignore
    uint16_t file_name_len;              // if cmd is 0 and 1, strlen(file_name); if cmd is 2, sizeof(sysbus_cfg_video_t)
    uint64_t start_timestamp;
    uint64_t end_timestamp;                  // if cmd is 2, ignore 
    uint8_t data[0];                     // variable length includ file_name string and tag string.
} sysbus_rec_cmd_msg_t;
#pragma  pack()

#pragma  pack(1)
typedef struct
{
    uint8_t sts;            // 0 - record started; 1 - record stopped; 2 - record segmented;
    uint8_t file_type;
    uint8_t file_sub_type;  //0 default 1 trigger    
    uint64_t trigger_time;  
    uint8_t base_seg;       //base seg flag      
    uint32_t file_size;     // file size in byte
    uint8_t file_name[0];   // variable length, '\0' end string
} sysbus_rec_sts_msg_t;
#pragma  pack()

#pragma  pack(1)
typedef struct
{
    uint32_t file_name_len;
    uint8_t file_name[0];  
} sysbus_base_seg_t;
#pragma  pack()

#pragma  pack(1)
typedef struct
{
    char status;   //0:recording  1: record the interrupt
    char channel;  //channel number(1~16) when record the interrupt
} sysbus_status_data_t;
#pragma  pack()

#pragma  pack(1)
typedef enum
{
    REC_COPY_DETECTED_USB,
    REC_COPY_CHECK_USBDISK,
    REC_COPY_CREATE_TARGET_DIR,
    REC_COPY_COPY_BEGIN,
    REC_COPY_COPY_COMPLETE,
    REC_COPY_LOCALDISK,
    REC_COPY_MAX_STATE
} sysbus_rec_copy_state_e;

typedef enum
{
    REC_COPY_SUCCEED,
    REC_COPY_ERR_USBDISK_INFO_PARSE_ERROR,
    REC_COPY_ERR_USBDISK_FS_CANT_SUPPORT,
    REC_COPY_ERR_USBDISK_MOUNT_ERROR,
    REC_COPY_ERR_USBDISK_LOST,
    REC_COPY_ERR_SOURCE_FILE_CANT_OPEN,
    REC_COPY_ERR_USBDISK_NOT_ENOUGH,
    REC_COPY_ERR_TARGET_DIR_CANT_CREATE,
    REC_COPY_ERR_TARGET_FILE_CANT_CREATE,
    REC_COPY_ERR_LOCALDISK_FULL,
    REC_COPY_MAX_ERRNO
} sysbus_rec_copy_errorno_e;

typedef struct
{
   sysbus_rec_copy_state_e copy_state;
   sysbus_rec_copy_errorno_e copy_errno;
} sysbus_rec_copy_data_t;

typedef enum
{
   COND_TRIG_START,
   COND_TRIG_STOP
} sysbus_cond_trig_state_e;

#define COND_NAME_MAX_LEN   (256)
typedef struct
{
    char cond_name[COND_NAME_MAX_LEN];
    char file_name[COND_NAME_MAX_LEN];
    sysbus_cond_trig_state_e status;
    int32_t  size;
}  sysbus_cond_trig_data_t;

typedef struct
{
    uint8_t file_name[COND_NAME_MAX_LEN];
} sysbus_split_t;

typedef sysbus_split_t sysbus_rec_download_t;

#pragma  pack()

#pragma  pack(1)
typedef struct
{
    uint64_t timestamp;
    uint8_t fault_code[8]; // 各字节值按照记录仪状态上报定义表填充
} sysbus_fault_code_msg_t;
#pragma  pack()

#ifdef __cplusplus
}
#endif

#endif
