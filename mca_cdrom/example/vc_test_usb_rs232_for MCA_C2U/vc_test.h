// vc_test.h : main header file for the VC_TEST application
//

#if !defined(AFX_VC_TEST_H__9133BA7F_6D98_47E4_A81C_EEDD7A25D35A__INCLUDED_)
#define AFX_VC_TEST_H__9133BA7F_6D98_47E4_A81C_EEDD7A25D35A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVc_testApp:
// See vc_test.cpp for the implementation of this class
//

class CVc_testApp : public CWinApp
{
public:
	CVc_testApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVc_testApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVc_testApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_TEST_H__9133BA7F_6D98_47E4_A81C_EEDD7A25D35A__INCLUDED_)
