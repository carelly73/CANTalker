#if !defined(AFX_TABINFO_H__A874A52F_E4E2_43C2_BDFB_BEE973A03387__INCLUDED_)
#define AFX_TABINFO_H__A874A52F_E4E2_43C2_BDFB_BEE973A03387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabInfo.h : header file
//

#include "type.h"

/////////////////////////////////////////////////////////////////////////////
// CTabInfo dialog
class CXTabCtrl;
class CTabInfo : public CDialog
{
// Construction
public:
	CTabInfo(CWnd* pParent = NULL);   // standard constructor
	CXTabCtrl  *m_pTab;
    Boolean    m_bRcvOn;

public:
	void DispData (PacketData *packet_p);
// Dialog Data
	//{{AFX_DATA(CTabInfo)
	enum { IDD = IDD_DLG_INFO_RCV_DATA };
	CString	m_packet_len;
	CString	m_packet_id;
	CString	m_packet_d7;
	CString	m_packet_d6;
	CString	m_packet_d5;
	CString	m_packet_d4;
	CString	m_packet_d3;
	CString	m_packet_d2;
	CString	m_packet_d1;
	CString	m_packet_d0;
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
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReqStatus();
	afx_msg void OnBtnClearData();
	afx_msg void OnBtnReqAutoSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSndPacket();
	afx_msg void OnChkTimeStamp();
	afx_msg void OnBtnReqErrorCount();
	afx_msg void OnBtnGetCount();
	afx_msg void OnBtnReqReqCanFilters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABINFO_H__A874A52F_E4E2_43C2_BDFB_BEE973A03387__INCLUDED_)
