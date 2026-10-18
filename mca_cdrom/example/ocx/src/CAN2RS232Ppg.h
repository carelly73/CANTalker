#if !defined(AFX_CAN2RS232PPG_H__46409ABB_28A1_46A7_A273_CE3CD3A1E6B0__INCLUDED_)
#define AFX_CAN2RS232PPG_H__46409ABB_28A1_46A7_A273_CE3CD3A1E6B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CAN2RS232Ppg.h : Declaration of the CCAN2RS232PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCAN2RS232PropPage : See CAN2RS232Ppg.cpp.cpp for implementation.

class CCAN2RS232PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCAN2RS232PropPage)
	DECLARE_OLECREATE_EX(CCAN2RS232PropPage)

// Constructor
public:
	CCAN2RS232PropPage();

// Dialog Data
	//{{AFX_DATA(CCAN2RS232PropPage)
	enum { IDD = IDD_PROPPAGE_CAN2RS232 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCAN2RS232PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2RS232PPG_H__46409ABB_28A1_46A7_A273_CE3CD3A1E6B0__INCLUDED)
