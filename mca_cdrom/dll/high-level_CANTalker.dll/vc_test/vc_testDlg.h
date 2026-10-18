// vc_testDlg.h : header file
//

#if !defined(AFX_VC_TESTDLG_H__43B4F58A_EE08_480C_8AA3_3F02C29D5424__INCLUDED_)
#define AFX_VC_TESTDLG_H__43B4F58A_EE08_480C_8AA3_3F02C29D5424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVc_testDlg dialog

class CVc_testDlg : public CDialog
{
// Construction
public:
	CVc_testDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVc_testDlg)
	enum { IDD = IDD_VC_TEST_DIALOG };
	CComboBox	m_cboIntf;
	CListBox	m_lstCan;
	int		m_baudrate;
	int		m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVc_testDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVc_testDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnInit();
	afx_msg void OnBtnGetInfo();
	afx_msg void OnClose();
	afx_msg void OnBtnSendData();
	afx_msg void OnBtnCanInit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnClear();
	afx_msg void OnBtnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_TESTDLG_H__43B4F58A_EE08_480C_8AA3_3F02C29D5424__INCLUDED_)
