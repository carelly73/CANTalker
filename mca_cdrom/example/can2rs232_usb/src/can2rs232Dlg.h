// can2rs232Dlg.h : header file
//

#if !defined(AFX_CAN2RS232DLG_H__7D331CD2_EBD8_4AB1_A7A3_C12BEEAB330A__INCLUDED_)
#define AFX_CAN2RS232DLG_H__7D331CD2_EBD8_4AB1_A7A3_C12BEEAB330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "can2rs232conv.h"
#include "tabinfo.h"
#include "tabsetup.h"
#include "XTabCtrl.h"
#include "tabrs232setup.h"
#include "obdmonitor.h"
#include "mpusbapi.h"
#include "CanOpenAnalyser.h"

#include "type.h"

/////////////////////////////////////////////////////////////////////////////
// CCan2rs232Dlg dialog
class CCan2rs232Dlg : public CDialog
{
public:
    CCAN2RS232Conv  m_converter;
	CTabInfo        m_TabInfo;
	CTabSetup       m_TabSetup;
    COBDMonitor     m_ObdMonitor;
	CCanOpenAnalyser m_CanOpenAnal;
	CString         m_inputCodeStr;
    CMpUsbAPI       m_mpUsbAPI;
    BOOL            m_bUsbOpen;

// Construction
public:
	CCan2rs232Dlg(CWnd* pParent = NULL);	// standard constructor
    void OnCommDataReady (WPARAM, LPARAM);
    void OnCommPacketDataReady (WPARAM, LPARAM);
	void OnCVRspSignal(WPARAM, LPARAM);
    void OnUsbDataReceived (WPARAM, LPARAM);

    void AddInfomation(char *fmt, ...);
   	void ClearInfomations(void);
    BOOL UsbDeviceOpen (void);    

// Dialog Data
	//{{AFX_DATA(CCan2rs232Dlg)
	enum { IDD = IDD_CAN2RS232_DIALOG };
	CListCtrl	m_infoList;
	CXTabCtrl	m_menuTabCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCan2rs232Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CCan2rs232Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBtnClearInfo();
	afx_msg void OnSelchangeMenuTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickMenuTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeviceRemoved (WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2RS232DLG_H__7D331CD2_EBD8_4AB1_A7A3_C12BEEAB330A__INCLUDED_)
