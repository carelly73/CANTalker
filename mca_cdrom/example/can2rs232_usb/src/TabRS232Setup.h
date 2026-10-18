#if !defined(AFX_TABRS232SETUP_H__A306998C_EF6A_4141_AD80_61CB7EDF0D5C__INCLUDED_)
#define AFX_TABRS232SETUP_H__A306998C_EF6A_4141_AD80_61CB7EDF0D5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRS232Setup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabRS232Setup dialog
class CXTabCtrl;
class CTabRS232Setup : public CDialog
{
// Construction
public:
	CTabRS232Setup(CWnd* pParent = NULL);   // standard constructor
	CXTabCtrl  *m_pTab;
// Dialog Data
	//{{AFX_DATA(CTabRS232Setup)
	enum { IDD = IDD_DLG_SETUP_RS232 };
	CComboBox	m_cboPort;
	CComboBox	m_cboRS232BaudRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRS232Setup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabRS232Setup)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRS232SETUP_H__A306998C_EF6A_4141_AD80_61CB7EDF0D5C__INCLUDED_)
