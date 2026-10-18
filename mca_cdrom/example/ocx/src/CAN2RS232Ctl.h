#if !defined(AFX_CAN2RS232CTL_H__86796823_1995_4B3A_AAAC_11897ED203D2__INCLUDED_)
#define AFX_CAN2RS232CTL_H__86796823_1995_4B3A_AAAC_11897ED203D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include  "CAN2RS232Conv.h"

// CAN2RS232Ctl.h : Declaration of the CCAN2RS232Ctrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl : See CAN2RS232Ctl.cpp for implementation.

class CCAN2RS232Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CCAN2RS232Ctrl)

// Constructor
public:
	CCAN2RS232Ctrl();
	CCAN2RS232Conv m_converter;

    void ProtocolAck (BOOL isError, Int16 ErrCode);
    void PacketReceived (Int32 id, Int8 length, Int8 *data);
	void RspCanStatus (Int8 status, Int32 canID, Int8 BC1, Int8 BC2, Int8 BC3);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAN2RS232Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCAN2RS232Ctrl();

	DECLARE_OLECREATE_EX(CCAN2RS232Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCAN2RS232Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCAN2RS232Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCAN2RS232Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCAN2RS232Ctrl)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCAN2RS232Ctrl)
	afx_msg BOOL Initialize(short port, long brate);
	afx_msg void Close();
	afx_msg BOOL SetCanID(BOOL extMode, long canID);
	afx_msg BOOL SetRs232BaudRate(long brate);
	afx_msg void SetCanBaudRate(short brate);
	afx_msg void ReqCanStatus();
	afx_msg BOOL SendMessage(long canID, short len, short data0, short data1, short data2, short data3, short data4, short data5, short data6, short data7);
	afx_msg void SetBitTiming(short BRGCON1, short BRGCON2, short BRGCON3);
	afx_msg void SetAcceptacneFilter(long mask1, long mask2, long filter1, long filter2, long filter3, long filter4, long filter5, long filter6);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCAN2RS232Ctrl)
	void FirePacketReceived(long canID, short length, short data0, short data1, short data2, short data3, short data4, short data5, short data6, short data7)
		{FireEvent(eventidPacketReceived,EVENT_PARAM(VTS_I4  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2), canID, length, data0, data1, data2, data3, data4, data5, data6, data7);}
	void FireProtocolAcknowlegde(BOOL isError, short ErrCode)
		{FireEvent(eventidProtocolAcknowlegde,EVENT_PARAM(VTS_BOOL  VTS_I2), isError, ErrCode);}
	void FireRspCanStauts(short status, long canID, short BRGCON1, short BRGCON2, short BRGCON3)
		{FireEvent(eventidRspCanStauts,EVENT_PARAM(VTS_I2  VTS_I4  VTS_I2  VTS_I2  VTS_I2), status, canID, BRGCON1, BRGCON2, BRGCON3);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCAN2RS232Ctrl)
	dispidInitialize = 1L,
	dispidClose = 2L,
	dispidSetCanID = 3L,
	dispidSetRs232BaudRate = 4L,
	dispidSetCanBaudRate = 5L,
	dispidReqCanStatus = 6L,
	dispidSendMessage = 7L,
	dispidSetBitTiming = 8L,
	dispidSetAcceptacneFilter = 9L,
	eventidPacketReceived = 1L,
	eventidProtocolAcknowlegde = 2L,
	eventidRspCanStauts = 3L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2RS232CTL_H__86796823_1995_4B3A_AAAC_11897ED203D2__INCLUDED)
