#if !defined(AFX_OBDMONITOR_H__6476D81A_003E_4834_9B76_73BDD980EC9D__INCLUDED_)
#define AFX_OBDMONITOR_H__6476D81A_003E_4834_9B76_73BDD980EC9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OBDMonitor.h : header file
//


#include "type.h"

/////////////////////////////////////////////////////////////////////////////
// COBDMonitor dialog
class CXTabCtrl;
class COBDMonitor : public CDialog
{
// Construction
public:
	COBDMonitor(CWnd* pParent = NULL);   // standard constructor
    void        SetDefaultValue (void);
    void        RcvDataProc (PacketData *packet_p);

    CXTabCtrl   *m_pTab;
	BOOL		m_bMonitorStart;
    
// Dialog Data
	//{{AFX_DATA(COBDMonitor)
	enum { IDD = IDD_DLG_OBD_SETUP };
	CListCtrl	m_monitorList;
	int		m_pollingInt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COBDMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COBDMonitor)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStart();
	afx_msg void OnBtnObdMode();
	afx_msg void OnChkEngineLoad();
	afx_msg void OnChkVehicleSpeed();
	afx_msg void OnChkTimingAdvance();
	afx_msg void OnChkIntakeAirTemp();
	afx_msg void OnChkMafAirFlowRate();
	afx_msg void OnChkThrottlePos();
	afx_msg void OnChkEngineTemp();
	afx_msg void OnChkFuelPressure();
	afx_msg void OnChkIntakeManPressure();
	afx_msg void OnChkEngineRpm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBDMONITOR_H__6476D81A_003E_4834_9B76_73BDD980EC9D__INCLUDED_)
