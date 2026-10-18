#if !defined(AFX_CAN2RS232_H__BE6F4081_F0FE_4357_87D5_0DE6D538EB52__INCLUDED_)
#define AFX_CAN2RS232_H__BE6F4081_F0FE_4357_87D5_0DE6D538EB52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CAN2RS232.h : main header file for CAN2RS232.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232App : See CAN2RS232.cpp for implementation.

class CCAN2RS232App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2RS232_H__BE6F4081_F0FE_4357_87D5_0DE6D538EB52__INCLUDED)
