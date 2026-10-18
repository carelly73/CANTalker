#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned long  Int32;
typedef unsigned short Int16;
typedef unsigned char  Int8;
typedef bool           Boolean;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;

#define WM_COMM_DATA_READY	         (WM_USER+1)
#define WM_COMM_PK_DATA_READY	     (WM_USER+2)
#define WM_COMM_PK_TIME_OUT          (WM_USER+3)
#define WM_CV_RSP_SIGNAL             (WM_USER+4)
#define WM_USBAPI_RCV_DATA           (WM_USER+5)
#define WM_DEVICE_REMOVED            (WM_USER+6)

#define PNULL                        NULL
#define RS232_MAX_DATA_BUFFER_SIZE   32
#define TOTAL_PACKET_BUFFER_SIZE     50
#define MAX_RCV_DATA_SIZE            4096

typedef enum
{
    RS232_STATE_IDLE,
    RS232_STATE_RCV_PID,
    RS232_STATE_RCV_LENGTH,
    RS232_STATE_RCV_DATA,
    RS232_STATE_RCV_CHECKSUM,
    RS232_STATE_RCV_EOT,
    RS232_STATE_TOTAL_NUM
}
RS232_STATE;

typedef enum
{
    PID_START_ID = 0x80,
    PID_SET_CAN_INIT = PID_START_ID,
    PID_SET_CAN_ID,
    PID_SET_CAN_BITRATE,
    PID_SET_RS232_BAUDRATE,
    PID_SEND_CAN_DATA,        // send from PC
    PID_RECEIVED_CAN_DATA,
    PID_REQ_CAN_STATUS,
    PID_RSP_CAN_STATUS,
    PID_IND_CAN_ERROR,
    PID_SET_CAN_PARAM,
    PID_SET_CLEAR_CAN_ERROR,
    PID_SET_CAN_FILTERS,
    PID_REQ_VERSION,    
    PID_RSP_VERSION,  
    
    PID_SET_CLOSE_C2E_CONNECTION = 0x90,
    PID_REQ_CAN_FILTERS,
    PID_RSP_CAN_FILTERS,    
    PID_REQ_DEVICE_INFO,
    PID_RSP_DEVICE_INFO,    
    PID_REQ_SPEED_INFO,
    PID_RSP_SPEED_INFO,
    PID_SET_TCPIP_INFO,    
    PID_REQ_TCPIP_INFO,
    PID_RSP_TCPIP_INFO,
    PID_REQ_TIME_STAMP,
    PID_RSP_TIME_STAMP,    

    PID_RECEIVED_EXT_CAN_DATA = 0xA0,
    PID_SET_CAN_RCV_MODE,
    PID_REQ_CAN_RCV_MODE,
    PID_RSP_CAN_RCV_MODE,
    
    PID_REQ_CAN_ERROR_COUNT,
    PID_RSP_CAN_ERROR_COUNT,
    
    PID_TOTAL_NUM,
    PID_END_ID = PID_TOTAL_NUM,

    PID_PROTOCOL_ACK = 0xC0,
    PID_PROTOCOL_NACK = 0xD0,
    PID_PROTOCOL_EOT = 0xE0,
    PID_PROTOCOL_HEADER = 0xF0
}
CAN2RS232_PID;

typedef struct PacketDataTag
{
    Int8 header;
    Int8 pid;
    Int8 length;
    Int8 *data;
    Int8 checksum;
    Int8 eot;
}
PacketData;

typedef enum
{
    RS232_BAUD_RATE_9600,
    RS232_BAUD_RATE_19200,
    RS232_BAUD_RATE_38400,
    RS232_BAUD_RATE_57600,
    RS232_BAUD_RATE_115200,
    RS232_BAUD_RATE_230400,
    RS232_BAUD_RATE_460800,
    RS232_BAUD_RATE_TOTAL_NUM
}
RS232_BAUD_RATE;

typedef enum
{    
    CAN_BAUD_RATE_125,
    CAN_BAUD_RATE_250,
    CAN_BAUD_RATE_500,
    CAN_BAUD_RATE_1000,
    CAN_BAUD_RATE_50,
    CAN_BAUD_RATE_100,
    CAN_BAUD_RATE_200,
    CAN_BAUD_RATE_400,    
    CAN_BAUD_RATE_USER,    
    CAN_BAUD_RATE_MAX_NO,
    CAN_BAUD_RATE_DEF
}
CAN_BAUD_RATE;

typedef enum
{
    CV_TIMER_WAIT_ACK = 200,
    CV_TIMER_SETUP_PROC,
}
CV_TIMER_ID;

Boolean getNumFromBuffer (char *buff, Int8 startPtr, Int32 &data, Int8 size);
#endif