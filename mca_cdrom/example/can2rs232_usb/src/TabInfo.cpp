// TabInfo.cpp : implementation file
//

#include "stdafx.h"
#include "can2rs232.h"
#include "TabInfo.h"
#include "mmsystem.h"
#include "Can2rs232Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_ID_AUTO_SEND  100
extern CCan2rs232Dlg *pMainDlg;

/////////////////////////////////////////////////////////////////////////////
// CTabInfo dialog


CTabInfo::CTabInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CTabInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabInfo)
	m_packet_len = _T("");
	m_packet_id = _T("");
	m_packet_d7 = _T("");
	m_packet_d6 = _T("");
	m_packet_d5 = _T("");
	m_packet_d4 = _T("");
	m_packet_d3 = _T("");
	m_packet_d2 = _T("");
	m_packet_d1 = _T("");
	m_packet_d0 = _T("");
	//}}AFX_DATA_INIT
}


void CTabInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabInfo)
	DDX_Text(pDX, IDC_EDIT_SNDPK_LEN, m_packet_len);
	DDX_Text(pDX, IDC_EDIT_SNDPK_ID, m_packet_id);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D7, m_packet_d7);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D6, m_packet_d6);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D5, m_packet_d5);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D4, m_packet_d4);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D3, m_packet_d3);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D2, m_packet_d2);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D1, m_packet_d1);
	DDX_Text(pDX, IDC_EDIT_SNDPK_D0, m_packet_d0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabInfo, CDialog)
	//{{AFX_MSG_MAP(CTabInfo)
	ON_BN_CLICKED(IDC_BTN_REQ_STATUS, OnBtnReqStatus)
	ON_BN_CLICKED(IDC_BTN_CLEAR_DATA, OnBtnClearData)
	ON_BN_CLICKED(IDC_BTN_REQ_AUTO_SEND, OnBtnReqAutoSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SND_PACKET, OnBtnSndPacket)
	ON_BN_CLICKED(IDC_CHK_TIME_STAMP, OnChkTimeStamp)
	ON_BN_CLICKED(IDC_BTN_REQ_ERROR_COUNT, OnBtnReqErrorCount)
	ON_BN_CLICKED(IDC_BTN_GET_COUNT, OnBtnGetCount)
	ON_BN_CLICKED(IDC_BTN_REQ_REQ_CAN_FILTERS, OnBtnReqReqCanFilters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabInfo message handlers

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabInfo::DispData (PacketData *packet_p)
{
    CString str, str2;
	Int8 length, *data;


    if (packet_p->pid == PID_RECEIVED_EXT_CAN_DATA)
    {
        length = packet_p->length - 8;
        data = packet_p->data + 8;
        str.Format ("%.2f", ((double)*(UINT32*)packet_p->data)/100.0);
        ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertItem (0, str, 0);        
        str.Format ("%08X", *(Int32*)(packet_p->data+4));
        ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetItemText(0,1,str);        
    }
    else
    {
        length = packet_p->length - 4;
    	data = packet_p->data + 4;
        str.Format ("%ld", timeGetTime());
        ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->InsertItem (0, str, 0);        
        str.Format ("%08X", *(Int32*)(packet_p->data));
        ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->SetItemText(0,1,str);        
    }

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

void CTabInfo::OnBtnReqStatus() 
{
	pMainDlg->m_converter.ReqCanStatus();
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

void CTabInfo::OnBtnReqAutoSend() 
{
	SetTimer (TIMER_ID_AUTO_SEND, 1, NULL);
	pMainDlg->AddInfomation ("%ld :Start Send Data !!", timeGetTime());	
}

void CTabInfo::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == TIMER_ID_AUTO_SEND)
	{	    
        static Int32 canId = 0;
        static Int8 cnter = 0;
        Int8 data[8];	
	    Int8 idx;
		Int32 lp = 100/20;

		while (lp--)
		{

        if (canId <= 0x7FF)//0x2000000)
		{
		    for (idx = 0; idx < 8; idx++)
			{
			    data[idx] = cnter++;
			}
            pMainDlg->m_converter.SendPacket (canId, 8, data);
		
		    canId++;
		}
	    else
		{
			canId = 0;
            cnter = 0;
            KillTimer (TIMER_ID_AUTO_SEND);
		    pMainDlg->AddInfomation ("%ld : Complete !!", timeGetTime());
			return;
		}

		}
	}
	
	CDialog::OnTimer(nIDEvent);
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
            data8[idx] = (Int8)dataI32[idx+2];
		}
        pMainDlg->m_converter.SendPacket (dataI32[0], (Int8)dataI32[1], data8);    		
	}
	else
	{
        AfxMessageBox ("Invalid Data !!");
	}
}

void CTabInfo::OnChkTimeStamp() 
{
    BOOL on = ((CButton*)GetDlgItem(IDC_CHK_TIME_STAMP))->GetCheck() == 1;
    pMainDlg->m_converter.SetTimeStamp (on);
}

void CTabInfo::OnBtnReqErrorCount() 
{
    pMainDlg->m_converter.ReqErrorCount ();
}
 

void CTabInfo::OnBtnGetCount() 
{
	CString str;
	str.Format ("%d", ((CListCtrl*)GetDlgItem(IDC_LIST_CTRL))->GetItemCount());
	AfxMessageBox (str);
}

void CTabInfo::OnBtnReqReqCanFilters() 
{
	pMainDlg->m_converter.ReqCanFilters ();
}
