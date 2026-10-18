// can2ethernetDlg.h : header file
//

#if !defined(AFX_CAN2ETHERNETDLG_H__E12F4912_9ECD_4BCF_8243_45F5EA76D34C__INCLUDED_)
#define AFX_CAN2ETHERNETDLG_H__E12F4912_9ECD_4BCF_8243_45F5EA76D34C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "CAN2EthernetDlg.h"
#include "CAN2RS232Conv.h"
#include "tabinfo.h"
#include "tabsetup.h"
#include "XTabCtrl.h"
#include "type.h"
#include "canopenanalyser1.h"
#include "MultiSendMsg.h"

/////////////////////////////////////////////////////////////////////////////
// CCan2ethernetDlg dialog

class CCan2ethernetDlg : public CDialog
{
// Construction
public:
	CCan2ethernetDlg(CWnd* pParent = NULL);	// standard constructor
    CTabInfo        m_TabInfo;
	CTabSetup       m_TabSetup;
    CCAN2RS232Conv  m_c2eConv;
    CCanOpenAnalyser m_CanOpenAnal;
    CMultiSendMsg   m_multiSndMsg;

    BOOL            m_bRcvCanOpen;
    
    void AddInfomation (char *str,...);
    void OnCommPacketDataReady (WPARAM wp, LPARAM lp);
    void OnCVRspSignal(WPARAM wparam, LPARAM lparam);
    void SetAllEnableTab (BOOL enable);
    void SaveINI (void);
    void LoadINI (void);
    void GetApplicationDir (CString &str);

// Dialog Data
	//{{AFX_DATA(CCan2ethernetDlg)
	enum { IDD = IDD_CAN2ETHERNET_DIALOG };
	CXTabCtrl	m_menuTabCtrl;
	CListCtrl	m_infoList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCan2ethernetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCan2ethernetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnClearInfo();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2ETHERNETDLG_H__E12F4912_9ECD_4BCF_8243_45F5EA76D34C__INCLUDED_)
