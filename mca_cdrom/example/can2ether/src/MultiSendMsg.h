#if !defined(AFX_MULTISENDMSG_H__CD179232_91A5_436E_82C5_E5363B822434__INCLUDED_)
#define AFX_MULTISENDMSG_H__CD179232_91A5_436E_82C5_E5363B822434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiSendMsg.h : header file
//

#include "type.h"
/////////////////////////////////////////////////////////////////////////////
// CMultiSendMsg dialog
class CXTabCtrl;
class CCan2ethernetDlg;
class CMultiSendMsg : public CDialog
{
// Construction
public:
	CMultiSendMsg(CWnd* pParent = NULL);   // standard constructor

	CXTabCtrl        *m_pTab;
    CCan2ethernetDlg *m_pCAN2EthernetDlg;

    void ParseAndSendMsg (char *pMsg);
    
// Dialog Data
	//{{AFX_DATA(CMultiSendMsg)
	enum { IDD = IDD_DLG_SEND_PACKET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSendMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiSendMsg)
	afx_msg void OnChkRcvCanOpen();
	afx_msg void OnBtnSendMsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISENDMSG_H__CD179232_91A5_436E_82C5_E5363B822434__INCLUDED_)
