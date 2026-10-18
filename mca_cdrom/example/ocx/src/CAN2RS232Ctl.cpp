// CAN2RS232Ctl.cpp : Implementation of the CCAN2RS232Ctrl ActiveX Control class.

#include "stdafx.h"
#include "CAN2RS232.h"
#include "CAN2RS232Ctl.h"
#include "CAN2RS232Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCAN2RS232Ctrl *pC2RCtrl = NULL;

IMPLEMENT_DYNCREATE(CCAN2RS232Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCAN2RS232Ctrl, COleControl)
	//{{AFX_MSG_MAP(CCAN2RS232Ctrl)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCAN2RS232Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCAN2RS232Ctrl)
	DISP_FUNCTION(CCAN2RS232Ctrl, "Initialize", Initialize, VT_BOOL, VTS_I2 VTS_I4)
	DISP_FUNCTION(CCAN2RS232Ctrl, "Close", Close, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SetCanID", SetCanID, VT_BOOL, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SetRs232BaudRate", SetRs232BaudRate, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SetCanBaudRate", SetCanBaudRate, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CCAN2RS232Ctrl, "ReqCanStatus", ReqCanStatus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SendMessage", SendMessage, VT_BOOL, VTS_I4 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SetBitTiming", SetBitTiming, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CCAN2RS232Ctrl, "SetAcceptacneFilter", SetAcceptacneFilter, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCAN2RS232Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCAN2RS232Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CCAN2RS232Ctrl)
	EVENT_CUSTOM("PacketReceived", FirePacketReceived, VTS_I4  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("ProtocolAcknowlegde", FireProtocolAcknowlegde, VTS_BOOL  VTS_I2)
	EVENT_CUSTOM("RspCanStauts", FireRspCanStauts, VTS_I2  VTS_I4  VTS_I2  VTS_I2  VTS_I2)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCAN2RS232Ctrl, 1)
	PROPPAGEID(CCAN2RS232PropPage::guid)
END_PROPPAGEIDS(CCAN2RS232Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCAN2RS232Ctrl, "CAN2RS232.CAN2RS232Ctrl.1",
	0x21c37a12, 0xa76d, 0x458b, 0xae, 0xd5, 0x1f, 0x16, 0x83, 0x5e, 0xec, 0xe0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCAN2RS232Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCAN2RS232 =
		{ 0xb32f5754, 0x4fa, 0x4d54, { 0x9e, 0x5d, 0xf1, 0x9a, 0x69, 0xd9, 0xa8, 0x80 } };
const IID BASED_CODE IID_DCAN2RS232Events =
		{ 0x1d3a5b8a, 0x6c8a, 0x4887, { 0xb8, 0x4b, 0xff, 0x1e, 0xbf, 0x82, 0xd2, 0x9e } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCAN2RS232OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCAN2RS232Ctrl, IDS_CAN2RS232, _dwCAN2RS232OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::CCAN2RS232CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCAN2RS232Ctrl

BOOL CCAN2RS232Ctrl::CCAN2RS232CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CAN2RS232,
			IDB_CAN2RS232,
			afxRegApartmentThreading,
			_dwCAN2RS232OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::CCAN2RS232Ctrl - Constructor

CCAN2RS232Ctrl::CCAN2RS232Ctrl()
{
	InitializeIIDs(&IID_DCAN2RS232, &IID_DCAN2RS232Events);
    pC2RCtrl = this;
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::~CCAN2RS232Ctrl - Destructor

CCAN2RS232Ctrl::~CCAN2RS232Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::OnDraw - Drawing function

void CCAN2RS232Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::DoPropExchange - Persistence support

void CCAN2RS232Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::OnResetState - Reset control to default state

void CCAN2RS232Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl::AboutBox - Display an "About" box to the user

void CCAN2RS232Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CAN2RS232);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232Ctrl message handlers

BOOL CCAN2RS232Ctrl::Initialize(short port, long brate) 
{
	BOOL flag = m_converter.Initialize ((Int8)port, brate);
	if (flag)
	{
		SetTimer (1, 50, NULL);
	}
	return flag;
}

void CCAN2RS232Ctrl::Close() 
{
	KillTimer (1);
	m_converter.Close();
}

BOOL CCAN2RS232Ctrl::SetCanID(BOOL extMode, long canID) 
{
    return m_converter.SetCANID (extMode, canID);
}

BOOL CCAN2RS232Ctrl::SetRs232BaudRate(long brate) 
{
	if (m_converter.isBaudRateCorrect(brate))
	{
        m_converter.SetRS232BaudRate ((RS232_BAUD_RATE)brate);
		return TRUE;
	}
	return FALSE;
}

void CCAN2RS232Ctrl::SetCanBaudRate(short brate) 
{
	m_converter.SetCANBaudRate ((CAN_BAUD_RATE)brate);
}

void CCAN2RS232Ctrl::ReqCanStatus() 
{
	m_converter.ReqCanStatus ();
}

BOOL CCAN2RS232Ctrl::SendMessage(long canID, short len, short data0, short data1, short data2, short data3, short data4, short data5, short data6, short data7) 
{
	Int8 data[8] = {0,};
	
	if (len > 8 || len < 0) 
	{
		return FALSE;
	}

	data[0] = (Int8)data0;
	data[1] = (Int8)data1;
	data[2] = (Int8)data2;
	data[3] = (Int8)data3;
	data[4] = (Int8)data4;
	data[5] = (Int8)data5;
	data[6] = (Int8)data6;
	data[7] = (Int8)data7;

	m_converter.SendPacket (canID, len, data);

	return TRUE;
}

void CCAN2RS232Ctrl::SetBitTiming(short BRGCON1, short BRGCON2, short BRGCON3) 
{
	Int8 bc[3] = { (Int8)BRGCON1, (Int8)BRGCON2, (Int8)BRGCON3 };
	m_converter.SetCANBitTiming (bc);
}

void CCAN2RS232Ctrl::SetAcceptacneFilter(long mask1, long mask2, long filter1, long filter2, long filter3, long filter4, long filter5, long filter6) 
{
    Int32 mask[2] = {mask1, mask2};
	Int32 filter[6] = {filter1, filter2, filter3, filter4, filter5, filter6};

	m_converter.SetFilters (mask, filter);
}

void CCAN2RS232Ctrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
        extern BOOL isDataReceived;
        if (isDataReceived)
		{
			m_converter.RxProc ();
            isDataReceived = FALSE;
		}
	}
	
	COleControl::OnTimer(nIDEvent);
}

void CCAN2RS232Ctrl::ProtocolAck (BOOL isError, Int16 ErrCode)
{
    FireProtocolAcknowlegde (isError, ErrCode);
}

void CCAN2RS232Ctrl::PacketReceived (Int32 id, Int8 length, Int8 *data)
{    
	Int8 buff[8] = {0,};
	memcpy (buff,data,length);
    FirePacketReceived (id, length, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);
}

void CCAN2RS232Ctrl::RspCanStatus (Int8 status, Int32 canID, Int8 BC1, Int8 BC2, Int8 BC3)
{
    FireRspCanStauts ((short)status, canID, (short)BC1, (short)BC2, (short)BC3);
}
