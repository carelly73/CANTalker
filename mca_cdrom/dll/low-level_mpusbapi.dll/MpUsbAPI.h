// MpUsbAPI.h: interface for the CMpUsbAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUSBAPI_H__ACC300D0_1EAC_4AC8_A1A1_F03CC5D0E10D__INCLUDED_)
#define AFX_MPUSBAPI_H__ACC300D0_1EAC_4AC8_A1A1_F03CC5D0E10D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "type.h"

#define DEVICE_VID_PID	"vid_04d8&pid_ffee"

class CMpUsbAPI  
{
public:
	CMpUsbAPI();
	virtual ~CMpUsbAPI();

	HINSTANCE	m_hLibHandle;
	HANDLE		m_hOutPipe;
	HANDLE		m_hInPipe;
    CWinThread  *m_hThread;
    BOOL        m_bRunThread;
    HWND        m_rcvMsgHwnd;

	BOOL LoadMpUsbAPIDll (HWND hwnd);
	BOOL OpenPipe (void);
	void ClosePipe (void);
	void CheckInvalidHandle (void);
	BOOL SendPacket (BYTE *data, UINT32 *pSize, UINT32 timeOut);
	BOOL RcvPacket (BYTE *data, UINT32 *pSize, UINT32 timeOut);
    BOOL CheckDeviceRemoved (MSG *pMsg);

    static UINT MonitorThread (LPVOID context);
};

#endif // !defined(AFX_MPUSBAPI_H__ACC300D0_1EAC_4AC8_A1A1_F03CC5D0E10D__INCLUDED_)
