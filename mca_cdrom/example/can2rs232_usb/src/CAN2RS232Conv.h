// CAN2RS232Conv.h: interface for the CCAN2RS232Conv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_)
#define AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "type.h"

class CCAN2RS232Conv  
{
public:
	CCAN2RS232Conv();
	virtual ~CCAN2RS232Conv();
	void RxProc (BYTE *packet, UINT32 size);
	void MainTask (void);
    void DestroyPacket(PacketData *packet_p);
    void PacketInterpreter(PacketData *packet_p);
    Int8 MakeCheckSumSub (PacketData *packet_p);
    void MakeCheckSum (PacketData *packet_p);
	//void SendPacket (void);
	void SendPacket (Int32 canID, Int8 len, Int8 *data);
	void RspCanStatus (PacketData *packet_p);
	void ReqCanStatus (void);
	void SetCANID (Boolean extMode, Int32 id);
    void SetRS232BaudRate (RS232_BAUD_RATE brate);
    void SetCANBaudRate (CAN_BAUD_RATE brate);
	void SetCANBitTiming (Int8 brgcon[3]);
	void SetFilters(Int32 Mask[2], Int32 Filter[6]);

	void SendPacketToUart(PacketData *packet_p);
    Boolean CreatePacket (PacketData *packet_p, CAN2RS232_PID pid, Int8 length);
	Boolean IsPacketDataCorrect (PacketData *packet_p);
    Boolean Initialize(HWND hwnd);
	void WaitAckSignal (CV_TIMER_ID id);
	void GeneralTimerProc(UINT id);
    void SetTimeStamp (BOOL on);
    void ReqErrorCount (void);
    void RspCanErrorCount (PacketData *packet_p);
	void ReqCanFilters (void);
	void RspCanFilters (PacketData *packet_p);

    void Close (void);

public:
	CComm         m_serial;
    PacketData    m_pkBuffer[TOTAL_PACKET_BUFFER_SIZE];
	Int8          fifoReadBuffer[IN_BUFFER_SIZE] ;
	Int8          m_pkBufferTailPtr;
    Int8          m_pkBufferHeadPtr;
	bool          m_bUartPortOpen;
	PacketData    m_pkLastData;
	Int8          m_retryNoForSendPacket;
    bool          m_AckPacketReceived;
    HWND          m_hwnd;
	RS232_STATE   m_uartState;
	Int8          m_RxDataLength;
	Int8          m_RxDataPtr;

	Int32         m_CAN_ID;
	Int8          m_BRGCON[3];
	Int32         m_maskFilter[2];
	Int32         m_AccptanceFilter[6];
	Int8          m_CAN_BaudRate;
	Int8          m_RS232_BaudRate;
	Int8          m_port;
	Boolean       m_bACK_signal;
	Boolean       m_bNACK_signal;
	Int8          m_timerCnter;
};

#endif // !defined(AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_)
