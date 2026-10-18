// can2rs232.h : main header file for the CAN2RS232 application
//

#if !defined(AFX_CAN2RS232_H__B61FF8ED_63A1_4C55_B29E_4FBEAF908617__INCLUDED_)
#define AFX_CAN2RS232_H__B61FF8ED_63A1_4C55_B29E_4FBEAF908617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// #define _SIMULATION_

/////////////////////////////////////////////////////////////////////////////
// CCan2rs232App:
// See can2rs232.cpp for the implementation of this class
//

class CCan2rs232App : public CWinApp
{
public:
	CCan2rs232App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCan2rs232App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCan2rs232App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2RS232_H__B61FF8ED_63A1_4C55_B29E_4FBEAF908617__INCLUDED_)
