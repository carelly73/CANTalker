#if !defined(AFX_TABINFO_H__6E4DF0A2_7A32_4EDA_8344_FD1A76C85C2B__INCLUDED_)
#define AFX_TABINFO_H__6E4DF0A2_7A32_4EDA_8344_FD1A76C85C2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabInfo.h : header file
//
#include "type.h"
/////////////////////////////////////////////////////////////////////////////
// CTabInfo dialog
class CXTabCtrl;
class CCan2ethernetDlg;
class CTabInfo : public CDialog
{
// Construction
public:
	CTabInfo(CWnd* pParent = NULL);   // standard constructor
	CXTabCtrl        *m_pTab;
    CCan2ethernetDlg *m_pCAN2EthernetDlg;
    bool             m_bRcvOn;

	void DispData (PacketData *packet_p);
    
// Dialog Data
	//{{AFX_DATA(CTabInfo)
	enum { IDD = IDD_DLG_INFO_RCV_DATA };
	CString	m_packet_id;
	CString	m_packet_len;
	CString	m_packet_d0;
	CString	m_packet_d1;
	CString	m_packet_d2;
	CString	m_packet_d3;
	CString	m_packet_d4;
	CString	m_packet_d5;
	CString	m_packet_d6;
	CString	m_packet_d7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabInfo)
	afx_msg void OnBtnReqStatus();
	afx_msg void OnBtnReqAutoSend();
	afx_msg void OnBtnClearData();
	afx_msg void OnChkReceive();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnReqVersion();
	afx_msg void OnBtnSndPacket();
	afx_msg void OnBtnErrClear();
	afx_msg void OnBtnGetCnt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABINFO_H__6E4DF0A2_7A32_4EDA_8344_FD1A76C85C2B__INCLUDED_)
