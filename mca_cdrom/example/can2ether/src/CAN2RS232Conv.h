// CAN2RS232Conv.h: interface for the CCAN2RS232Conv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_)
#define AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "type.h"
#include "CAN2EthernetSock.h"

class CCan2ethernetDlg;

class CCAN2RS232Conv  
{
public:
	CCAN2RS232Conv();
	virtual ~CCAN2RS232Conv();
	void RxProc (Int8 *fifoReadBuffer, int fifoReadCount);
	void MainTask (void);
    void DestroyPacket(PacketData *packet_p);
    void PacketInterpreter(PacketData *packet_p);
    Int8 MakeCheckSumSub (PacketData *packet_p);
    void MakeCheckSum (PacketData *packet_p);
	//void SendPacket (void);
	void SendPacket (Int32 canID, Int8 len, Int8 *data);
	void RspCanStatus (PacketData *packet_p);
	void ReqCanStatus (void);
    void CmdErrClear (void);
	void SetCANID (Boolean extMode, Int32 id);
    void SetRS232BaudRate (RS232_BAUD_RATE brate);
    void SetCANBaudRate (CAN_BAUD_RATE brate);
	void SetCANBitTiming (Int8 brgcon[3]);
	void SetFilters(Int32 Mask[2], Int32 Filter[6]);
	void ReqVersion (void);
	void RspFirmwareVersion (PacketData *packet_p);
	void CloseConnection (void);

	void SendPacketToSocket(PacketData *packet_p);
    Boolean CreatePacket (PacketData *packet_p, CAN2RS232_PID pid, Int8 length);
	Boolean IsPacketDataCorrect (PacketData *packet_p);
    void Initialize(HWND hwnd, CString ipaddr, Int16 port);
	void WaitAckSignal (CV_TIMER_ID id);
	void GeneralTimerProc(UINT id);

    void Close (void);

public:
    CCAN2EthernetSock m_c2eSock;
    CCan2ethernetDlg  *m_pCAN2EthernetDlg;

    PacketData    m_pkBuffer[TOTAL_PACKET_BUFFER_SIZE];
	Int8          m_pkBufferTailPtr;
    Int8          m_pkBufferHeadPtr;
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
	Boolean       m_bACK_signal;
	Boolean       m_bNACK_signal;
	Int8          m_timerCnter;

    Int8          m_ipAddr[4];
    Int16         m_port;
    bool          m_isOnline;
	
};

#endif // !defined(AFX_CAN2RS232CONV_H__625F84FC_8C63_4487_972A_CADC086FDA2F__INCLUDED_)
