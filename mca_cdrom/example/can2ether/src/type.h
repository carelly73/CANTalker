#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned short UINT16;
typedef unsigned char  UINT8;
typedef unsigned long  Int32;
typedef unsigned short Int16;
typedef unsigned char  Int8;
typedef bool           Boolean;

#define WM_COMM_DATA_READY	         (WM_USER+100)
#define WM_COMM_PK_DATA_READY	     (WM_USER+101)
#define WM_COMM_PK_TIME_OUT          (WM_USER+102)
#define WM_CV_RSP_SIGNAL             (WM_USER+103)

#define PNULL                        NULL
#define RS232_MAX_DATA_BUFFER_SIZE   20
#define TOTAL_PACKET_BUFFER_SIZE     50

typedef enum
{
    RS232_STATE_RCV_PID,
    RS232_STATE_RCV_LENGTH,
    RS232_STATE_RCV_DATA,
    RS232_STATE_RCV_CHECKSUM,
    RS232_STATE_TOTAL_NUM
}
RS232_STATE;

typedef enum
{
    PID_START_ID = 0x80,
    PID_CAN_INIT = PID_START_ID,
    PID_CAN_SET_ID,
    PID_CAN_SET_BAUD_RATE,
    PID_RS232_SET_BAUD_RATE,
    PID_CAN_SEND_DATA,        // send from PC
    PID_CAN_RECEIVED_DATA,
    PID_CAN_REQ_STATUS,
    PID_CAN_RSP_STATUS,
    PID_CAN_IND_ERROR,
    PID_CAN_SET_COMM_PARAM,
    PID_CAN_CMD_CLEAR_ERROR,
	PID_CAN_FILTERS,
	PID_REQ_VERSION,    
    PID_RSP_VERSION, 
    
	PID_ETH_CLOSE_CONNECTION = 0x90,

    PID_PROTOCOL_ACK = 0xC0,
    PID_PROTOCOL_NACK = 0xD0,

    PID_TOTAL_NUM,
    PID_END_ID = PID_TOTAL_NUM,

    PID_PROTOCOL_EOT = 0xE0,
    PID_PROTOCOL_HEADER = 0xF0
}
CAN2RS232_PID;

typedef struct PacketDataTag
{
    Int8 pid;
    Int8 length;
    Int8 *data;
    Int8 checksum;
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
    RS232_BAUD_RATE_TOTAL_NUM
}
RS232_BAUD_RATE;

typedef enum
{
    CAN_BAUD_RATE_125,
    CAN_BAUD_RATE_250,
    CAN_BAUD_RATE_500,
    CAN_BAUD_RATE_1000,
    CAN_BAUD_RATE_USER,
    CAN_BAUD_RATE_MAX_NO    
}
CAN_BAUD_RATE;

typedef enum
{
    CV_TIMER_WAIT_ACK = 200,
    CV_TIMER_SETUP_PROC,
	CV_TIMER_CHECK_SERIAL_BUFFER
}
CV_TIMER_ID;
#endif