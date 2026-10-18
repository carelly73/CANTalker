#if !defined(AFX_TABSETUP_H__0C228213_D4F0_4FF2_838F_740D878F0854__INCLUDED_)
#define AFX_TABSETUP_H__0C228213_D4F0_4FF2_838F_740D878F0854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabSetup.h : header file
//

#include "type.h"

/////////////////////////////////////////////////////////////////////////////
// CTabSetup dialog
class CXTabCtrl;
class CCan2ethernetDlg;
class CTabSetup : public CDialog
{
// Construction
public:
	void SetDefaultValue (void);    
    Int8 HexToDec (char code);
	Boolean isNumeric (char &ascii);
    Boolean getNumFromBuffer (char *buff, Int8 startPtr, Int32 &data, Int8 size);
	void SetupProcHandler (void);
    void SetCANID (void);
    void SetRS232BaudRate (void);
    void SetCANBaudRate (void);
    void SetFilterMask (void);
	void SetBitTiming (void);
    
	CTabSetup(CWnd* pParent = NULL);   // standard constructor
	CXTabCtrl        *m_pTab;
    CCan2ethernetDlg *m_pCAN2EthernetDlg;
	Int8             m_setupProcMode;
	Int8             m_ACK_NACK;
	Int8             m_detectSpeed;
    
// Dialog Data
	//{{AFX_DATA(CTabSetup)
	enum { IDD = IDD_DLG_SETTING };
	CComboBox	m_cboCANBaudRate;
	CString	m_CAN_ID;
	CString	m_IP_Addr;
	CString	m_RX0FILTER0;
	CString	m_RX0FILTER1;
	CString	m_RX0MASK;
	CString	m_RX1FILTER2;
	CString	m_RX1FILTER3;
	CString	m_RX1FILTER4;
	CString	m_RX1FILTER5;
	CString	m_RX1MASK;
	CString	m_BRGCON1;
	CString	m_BRGCON2;
	CString	m_BRGCON3;
	UINT	m_portNo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSetup)
	afx_msg void OnBtnSetting();
	afx_msg void OnBtnConnect();
	afx_msg void OnChkUserFilter();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSETUP_H__0C228213_D4F0_4FF2_838F_740D878F0854__INCLUDED_)
