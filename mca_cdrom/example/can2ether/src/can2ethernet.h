// can2ethernet.h : main header file for the CAN2ETHERNET application
//

#if !defined(AFX_CAN2ETHERNET_H__682A1691_C1CA_4BEA_9D55_AF18ADBF18BB__INCLUDED_)
#define AFX_CAN2ETHERNET_H__682A1691_C1CA_4BEA_9D55_AF18ADBF18BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//#define _SIMULATION_

/////////////////////////////////////////////////////////////////////////////
// CCan2ethernetApp:
// See can2ethernet.cpp for the implementation of this class
//

class CCan2ethernetApp : public CWinApp
{
public:
	CCan2ethernetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCan2ethernetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCan2ethernetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2ETHERNET_H__682A1691_C1CA_4BEA_9D55_AF18ADBF18BB__INCLUDED_)
