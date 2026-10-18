// TabInfo.cpp : implementation file
//

#include "stdafx.h"
#include "can2ethernet.h"
#include "TabInfo.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabInfo dialog
#include "can2ethernetDlg.h"
#define TIMER_ID_AUTO_SEND  100

CTabInfo::CTabInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CTabInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabInfo)
	m_packet_id = _T("");
	m_packet_len = _T("");
	m_packet_d0 = _T("");
	m_packet_d1 = _T("");
	m_packet_d2 = _T("");
	m_packet_d3 = _T("");
	m_packet_d4 = _T("");
	m_packet_d5 = _T("");
	m_packet_d6 = _T("");
	m_packet_d7 = _T("");
	//}}AFX_DATA_INIT
}


void CTabInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabInfo)
	DDX_Text(pDX, IDC_EDIT_SND_PK_ID, m_packet_id);
	DDX_Text(pDX, IDC_EDIT_SND_PK_LEN, m_packet_len);
	DDX_Text(pDX, IDC_EDIT_SND_PK0, m_packet_d0);
	DDX_Text(pDX, IDC_EDIT_SND_PK1, m_packet_d1);
	DDX_Text(pDX, IDC_EDIT_SND_PK2, m_packet_d2);
	DDX_Text(pDX, IDC_EDIT_SND_PK3, m_packet_d3);
	DDX_Text(pDX, IDC_EDIT_SND_PK4, m_packet_d4);
	DDX_Text(pDX, IDC_EDIT_SND_PK5, m_packet_d5);
	DDX_Text(pDX, IDC_EDIT_SND_PK6, m_packet_d6);
	DDX_Text(pDX, IDC_EDIT_SND_PK7, m_packet_d7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabInfo, CDialog)
	//{{AFX_MSG_MAP(CTabInfo)
	ON_BN_CLICKED(IDC_BTN_REQ_STATUS, OnBtnReqStatus)
	ON_BN_CLICKED(IDC_BTN_REQ_AUTO_SEND, OnBtnReqAutoSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR_DATA, OnBtnClearData)
	ON_BN_CLICKED(IDC_CHK_RECEIVE, OnChkReceive)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_REQ_VERSION, OnBtnReqVersion)
	ON_BN_CLICKED(IDC_BTN_SND_PACKET, OnBtnSndPacket)
	ON_BN_CLICKED(IDC_BTN_ERR_CLEAR, OnBtnErrClear)
	ON_BN_CLICKED(IDC_BTN_GET_CNT, OnBtnGetCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabInfo message handlers

void CTabInfo::OnBtnReqStatus() 
{
	m_pCAN2EthernetDlg->m_c2eConv.ReqCanStatus ();
}

void CTabInfo::OnBtnReqAutoSend() 
{
	SetTimer (TIMER_ID_AUTO_SEND, 1, NULL);
	m_pCAN2EthernetDlg->AddInfomation ("Start Send Data !!");	    
}

void CTabInfo::OnBtnClearData() 
{
	int total;
    
	total = ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->GetItemCount ();
	while (total--)
	{
	    ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->DeleteItem (total);
	}
}

void CTabInfo::OnChkReceive() 
{
	m_bRcvOn = ((CButton*)GetDlgItem(IDC_CHK_RECEIVE))->GetCheck() == 1;
}

BOOL CTabInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB;
	((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetExtendedStyle (dwStyle);

    ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertColumn (1, "Time Stamp",LVCFMT_CENTER,90);
	((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertColumn (2, "ID",LVCFMT_CENTER,80);
	((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertColumn (3, "Len",LVCFMT_CENTER,40);
	((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertColumn (4, "Data",LVCFMT_LEFT,180);

	m_packet_id = "00000000";
	m_packet_len = "0";
	m_packet_d0 = "0";
	m_packet_d1 = "0";
	m_packet_d2 = "0";
	m_packet_d3 = "0";
	m_packet_d4 = "0";
	m_packet_d5 = "0";
	m_packet_d6 = "0";
	m_packet_d7 = "0";
	UpdateData (false);

	m_bRcvOn = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabInfo::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == TIMER_ID_AUTO_SEND)
	{	    
        static Int32 canId = 0;
        static Int8 cnter = 0;
        Int8 data[8];	
	    Int8 idx;
        Int8 lp=15;

        if (canId <= 0x7ff)
		{
			while ((lp--) && (canId <= 0x7FF))
			{
		    for (idx = 0; idx < 8; idx++)
			{
			    data[idx] = cnter++;
			}
            m_pCAN2EthernetDlg->m_c2eConv.SendPacket (canId, 8, data);
		
		    canId++;
		}
		}
	    else
		{
			canId = 0;
            cnter = 0;
            KillTimer (TIMER_ID_AUTO_SEND);
		    m_pCAN2EthernetDlg->AddInfomation ("Complete !!");
		}
	}	
	CDialog::OnTimer(nIDEvent);
}

void CTabInfo::OnBtnReqVersion() 
{
	m_pCAN2EthernetDlg->m_c2eConv.ReqVersion ();
}

void CTabInfo::DispData (PacketData *packet_p)
{
    CString str, str2;
	Int8 length, *data;

	if (!m_bRcvOn) return;	
	
    length = packet_p->length - 4;
	data = packet_p->data + 4;

	str.Format ("%ld", timeGetTime());
	((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertItem (0, str, 0);
	str.Format ("%08X", *(Int32*)(packet_p->data));
    ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetItemText(0,1,str);
	str.Format ("%d", length);
    ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetItemText(0,2,str);

    if (length > 0)
	{
	    while (length--)
		{
		    str.Format("%02X ",*data++);
            str2.Insert(str2.GetLength(),str);
		}
	}
    ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetItemText(0,3,str2);
}


Int8 HexToDec (char code)
{
    if (code >= '0' && code <= '9')
    {
        return code - '0';
    }
    else if (code >= 'A' && code <= 'F')
    {
        return code - 'A' + 10;
    }
    else if (code >= 'a' && code <= 'f')
    {
        return code - 'a' + 10;
    }
    return 0xff;
}


Boolean getNumFromBuffer (char *buff, Int8 startPtr, Int32 &data, Int8 size)
{
    Int8  idx;
    Int32 temp = 0;
    Int32 temp2;
    Int8  bit;
    Int8 endPtr;

    bit = size * 4 - 4;
    endPtr = startPtr + size;

    for (idx = startPtr; idx < endPtr; idx++)
    {
        temp2 = HexToDec (buff[idx]);
        if (temp2 < 0xff)
        {
            temp += temp2 << bit;
            bit -= 4;
        }
        else
        {
            return FALSE;
        }
    }
    data = temp;
    return TRUE;
}

void CTabInfo::OnBtnSndPacket() 
{
	UpdateData (true);

	CString *dataB[] = { &m_packet_id, &m_packet_len, &m_packet_d0, &m_packet_d1, &m_packet_d2,
		&m_packet_d3, &m_packet_d4, &m_packet_d5, &m_packet_d6, &m_packet_d7 };

    Int32 dataI32[10];
	Int8  data8[8];
	char  buff[20];
	int   idx;

	for (idx = 0; idx < 10; idx++)
	{
		dataB[idx]->MakeUpper();
		sprintf (buff, "%s", *dataB[idx]);
	    if (!getNumFromBuffer (buff,0, dataI32[idx], strlen (buff)))
		{
            break;
		}	
	}

	if (idx == 10)
	{
        for (idx = 0; idx < 8; idx++)
		{
            data8[idx] = (UINT8)dataI32[idx+2];
		}
        m_pCAN2EthernetDlg->m_c2eConv.SendPacket (dataI32[0], (UINT8)dataI32[1], data8);    		
	}
	else
	{
        AfxMessageBox ("Invalid Data !!");
	}	
}

void CTabInfo::OnBtnErrClear() 
{
	m_pCAN2EthernetDlg->m_c2eConv.CmdErrClear ();	
}


void CTabInfo::OnBtnGetCnt() 
{
    int total = ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->GetItemCount();
    CString str;

    str.Format ("%d", total);
    AfxMessageBox (str);
}
