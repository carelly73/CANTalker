// MultiSendMsg.cpp : implementation file
//

#include "stdafx.h"
#include "can2ethernet.h"
#include "MultiSendMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSendMsg dialog

#include "can2ethernetDlg.h"

extern Boolean getNumFromBuffer (char *buff, Int8 startPtr, Int32 &data, Int8 size);

CMultiSendMsg::CMultiSendMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiSendMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiSendMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMultiSendMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSendMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiSendMsg, CDialog)
	//{{AFX_MSG_MAP(CMultiSendMsg)
	ON_BN_CLICKED(IDC_CHK_RCV_CAN_OPEN, OnChkRcvCanOpen)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, OnBtnSendMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSendMsg message handlers

void CMultiSendMsg::OnChkRcvCanOpen() 
{
	m_pCAN2EthernetDlg->m_bRcvCanOpen = 
        ((CButton*)GetDlgItem(IDC_CHK_RCV_CAN_OPEN))->GetCheck() == 1;    
}

void CMultiSendMsg::ParseAndSendMsg (char *pMsg)
{
    Int32   canID, i, temp32;
    Int8    data[8], len = 0;
    char    *tok;
    
    if (pMsg == NULL || pMsg[0] == NULL)
    {
        return;
    }
    
    tok = strtok (pMsg, ",");
    if (tok == NULL)
    {
        return;
    }
    if (!getNumFromBuffer(tok, 0, canID, strlen(tok)))
    {
        return;
    }

    tok = strtok (NULL, ",");
    if (tok == NULL)
    {
        return;
    }
    if (!getNumFromBuffer(tok, 0, temp32, strlen(tok)))
    {
        return;
    }

    len = (Int8)temp32;

    if (len > 0)
    {
        for (i = 0; i < len; i++)
        {
            tok = strtok (NULL, ",");
            if (tok == NULL)
            {
                return;
            }
            if (!getNumFromBuffer(tok, 0, temp32, strlen(tok)))
            {
                return;
            }
            data[i] = (UINT8)temp32;
        }
    }
    m_pCAN2EthernetDlg->m_c2eConv.SendPacket (canID, len, data);
}

void CMultiSendMsg::OnBtnSendMsg() 
{
	const UINT32 chkList[] = 
    { 
        IDC_CHK_MSG1, IDC_CHK_MSG2, IDC_CHK_MSG3, IDC_CHK_MSG4, IDC_CHK_MSG5,
        IDC_CHK_MSG6, IDC_CHK_MSG7, IDC_CHK_MSG8, IDC_CHK_MSG9, IDC_CHK_MSG10
	};
            
    const UINT32 msgList[] = 
    { 
        IDC_EDIT_MSG1, IDC_EDIT_MSG2, IDC_EDIT_MSG3, IDC_EDIT_MSG4, IDC_EDIT_MSG5,
        IDC_EDIT_MSG6, IDC_EDIT_MSG7, IDC_EDIT_MSG8, IDC_EDIT_MSG9, IDC_EDIT_MSG10
	};

    UINT32 i;

    for (i = 0; i < 10; i++)
    {
        if (((CButton*)GetDlgItem(chkList[i]))->GetCheck() == 1)
        {
            char buff[512] = {0,};
            GetDlgItem(msgList[i])->GetWindowText (buff, sizeof(buff));
            ParseAndSendMsg (buff);
        }
    }
}
