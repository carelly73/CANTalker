#if !defined(AFX_CANOPENANALYSER_H__29FEA0DD_9CD9_4C00_B371_145FE72B3D38__INCLUDED_)
#define AFX_CANOPENANALYSER_H__29FEA0DD_9CD9_4C00_B371_145FE72B3D38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanOpenAnalyser.h : header file
//

#include "type.h"
#include "xlistbox.h"

typedef struct _LIST_ITEM
{
	struct _LIST_ITEM *Prev;
	struct _LIST_ITEM *Next;
} 
LIST_ITEM, *PLIST_ITEM;

typedef struct _LINKED_LIST_TAG
{
	PLIST_ITEM          Head;
	PLIST_ITEM          Tail;
    CRITICAL_SECTION	cs;
} 
LINKED_LIST;

/* CANopen NMT constants */
/*  States */
#define NMT_INITIALIZING            0
#define NMT_PRE_OPERATIONAL         127
#define NMT_OPERATIONAL             5
#define NMT_STOPPED                 4

/* commands */
#define NMT_ENTER_OPERATIONAL       1
#define NMT_ENTER_STOPPED           2
#define NMT_ENTER_PRE_OPERATIONAL   128
#define NMT_RESET_NODE              129
#define NMT_RESET_COMMUNICATION     130

#define COB_ID_FUNC_CODE_MASK       0x780
#define COB_ID_NODE_IDMASK          0x07F

#define FUNC_CODE_NMT_SERVICE       0x000
#define FUNC_CODE_SYNC              0x080
#define FUNC_CODE_EMERGENCY         0x080 
#define FUNC_CODE_TIME_STAMP        0x100
#define FUNC_CODE_TPDO0             0x180 
#define FUNC_CODE_RPDO0             0x200 
#define FUNC_CODE_TPDO1             0x280 
#define FUNC_CODE_RPDO1             0x300 
#define FUNC_CODE_TPDO2             0x380 
#define FUNC_CODE_RPDO2             0x400 
#define FUNC_CODE_TPDO3             0x480 
#define FUNC_CODE_RPDO3             0x500 
#define FUNC_CODE_TSDO              0x580 
#define FUNC_CODE_RSDO              0x600 
#define FUNC_CODE_ERROR_CONTROL     0x700 

#define SDO_CMD_MASK                0xE0
#define SDO_CMD_INIT_DOWNLOAD_CS    0x020
#define SDO_CMD_INIT_DOWNLOAD_SC    0x160
#define SDO_CMD_DOWNLOAD_SEG_CS     0x000
#define SDO_CMD_DOWNLOAD_SEG_SC     0x101
#define SDO_CMD_INIT_UPLOAD_CS      0x040
#define SDO_CMD_INIT_UPLOAD_SC      0x140
#define SDO_CMD_UPLOAD_SEG_CS       0x060
#define SDO_CMD_UPLOAD_SEG_SC       0x100
#define SDO_CMD_ABORT_TRANSFER_CS   0x080
#define SDO_CMD_ABORT_TRANSFER_SC   0x180

#define T_WARN      0
#define T_ERROR     1
#define T_INFO1     2
#define T_INFO2     3
#define T_NORMAL    4

typedef struct
{
    UINT32  abortCode;
    char    *abortDescription;
}
ABORT_INFO;

typedef struct
{
    UINT16  errCode;
    UINT16  errMask;
    char    *meanStr;
}
EMER_INFO;

typedef struct
{
    UINT32  timeStamp;
    UINT16  canId;
    UINT8   rtr;
    UINT8   dlc;
    UINT8   data[8];        
}
CAN_MSG;

typedef struct
{
	LIST_ITEM   link;
	CAN_MSG		msg;
}
CAN_MSG_LIST;

typedef enum
{
    ERR_NONE
}
ERR_CODE;

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalyser dialog
class CXTabCtrl;
class CCanOpenAnalyser : public CDialog
{
// Construction
public:
	CCanOpenAnalyser(CWnd* pParent = NULL);   // standard constructor
	CXTabCtrl   *m_pTab;
	BOOL		m_bAutoScroll;
	LINKED_LIST	m_canMsgList;
    UINT32      m_baseTime;
    CFont       m_font;
    BOOL        m_bAlign;
	
	void EnQMsg (CAN_MSG *pCanMsg);
	BOOL DeQMsg (CAN_MSG *pCanMsg);
	void UpdateCanMsg (void);
    void RcvDataProc (PacketData *packet_p);

// Dialog Data
	//{{AFX_DATA(CCanOpenAnalyser)
	enum { IDD = IDD_DLG_CAN_OPEN };
	CXListBox	m_lstInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanOpenAnalyser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanOpenAnalyser)
	afx_msg void OnBtnClear();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkAutoscroll();
	afx_msg void OnBtnSave();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChkAlign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANOPENANALYSER_H__29FEA0DD_9CD9_4C00_B371_145FE72B3D38__INCLUDED_)
