// TabSetup.cpp : implementation file
//

#include "stdafx.h"
#include "can2ethernet.h"
#include "TabSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSetup dialog
#include "can2ethernetDlg.h"

#define TIMER_ID_WAIT_CONNECTION    100
#define TIMER_ID_WAIT_DISCONNECTION 101

CTabSetup::CTabSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CTabSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSetup)
	m_CAN_ID = _T("");
	m_IP_Addr = _T("");
	m_RX0FILTER0 = _T("");
	m_RX0FILTER1 = _T("");
	m_RX0MASK = _T("");
	m_RX1FILTER2 = _T("");
	m_RX1FILTER3 = _T("");
	m_RX1FILTER4 = _T("");
	m_RX1FILTER5 = _T("");
	m_RX1MASK = _T("");
	m_BRGCON1 = _T("");
	m_BRGCON2 = _T("");
	m_BRGCON3 = _T("");
	m_portNo = 0;
	//}}AFX_DATA_INIT
}


void CTabSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSetup)
	DDX_Control(pDX, IDC_CBO_CAN, m_cboCANBaudRate);
	DDX_Text(pDX, IDC_EDIT_CAN_ID, m_CAN_ID);
	DDX_Text(pDX, IDC_EDIT_IP_ADDR, m_IP_Addr);
	DDX_Text(pDX, IDC_EDIT_RX0FILTER0, m_RX0FILTER0);
	DDX_Text(pDX, IDC_EDIT_RX0FILTER1, m_RX0FILTER1);
	DDX_Text(pDX, IDC_EDIT_RX0MASK, m_RX0MASK);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER2, m_RX1FILTER2);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER3, m_RX1FILTER3);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER4, m_RX1FILTER4);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER5, m_RX1FILTER5);
	DDX_Text(pDX, IDC_EDIT_RX1MASK, m_RX1MASK);
	DDX_Text(pDX, IDC_EDIT_BRGCON1, m_BRGCON1);
	DDX_Text(pDX, IDC_EDIT_BRGCON2, m_BRGCON2);
	DDX_Text(pDX, IDC_EDIT_BRGCON3, m_BRGCON3);
	DDX_Text(pDX, IDC_EDIT_PORT_NO, m_portNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSetup, CDialog)
	//{{AFX_MSG_MAP(CTabSetup)
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_CHK_USER_FILTER, OnChkUserFilter)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSetup message handlers

void CTabSetup::OnBtnSetting() 
{
    m_setupProcMode = 0;
	m_ACK_NACK = 0;
	SetCANID ();
	return;	
}

void CTabSetup::SetCANID (void)
{
	char buffer[20];
	Int32 data;
	Boolean extMode;

	UpdateData (true);
	m_CAN_ID.MakeUpper();
	extMode = ((CButton*)GetDlgItem(IDC_CHK_EXT_ID_MODE))->GetCheck() == 1;

	sprintf (buffer,"%s", m_CAN_ID);

    if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
	{
        m_pCAN2EthernetDlg->m_c2eConv.SetCANID (extMode, data);	
	}
	else
	{
        m_pCAN2EthernetDlg->AddInfomation ("Invalid CAN ID=%s", buffer);
	}
}

void CTabSetup::SetCANBaudRate (void)
{
	UpdateData (true);
	m_pCAN2EthernetDlg->m_c2eConv.SetCANBaudRate ((CAN_BAUD_RATE)m_cboCANBaudRate.GetCurSel());
}

void CTabSetup::SetFilterMask (void)
{
    char buffer[20];
	Int32 data;
	Int32 mask[2], filter[6];

	UpdateData (true);
	m_RX0MASK.MakeUpper();
	m_RX1MASK.MakeUpper();
	m_RX0FILTER0.MakeUpper();
	m_RX0FILTER1.MakeUpper();
	m_RX1FILTER2.MakeUpper();
	m_RX1FILTER3.MakeUpper();
	m_RX1FILTER4.MakeUpper();
	m_RX1FILTER5.MakeUpper();

	sprintf (buffer,"%s", m_RX0MASK);

    if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
	{
		mask[0] = data;
		sprintf (buffer,"%s", m_RX1MASK);
        if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
		{
            mask[1] = data;
            sprintf (buffer,"%s", m_RX0FILTER0);
			if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
			{
				filter[0] = data;
                sprintf (buffer,"%s", m_RX0FILTER1);
			    if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
				{
					filter[1] = data;
                    sprintf (buffer,"%s", m_RX1FILTER2);
			        if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
					{
						filter[2] = data;
					    sprintf (buffer,"%s", m_RX1FILTER3);
			            if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
						{
							filter[3] = data;
							sprintf (buffer,"%s", m_RX1FILTER4);
			                if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
							{
								filter[4] = data;
								sprintf (buffer,"%s", m_RX1FILTER5);
			                    if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
								{
									filter[5] = data;									
				                    m_pCAN2EthernetDlg->m_c2eConv.SetFilters (mask, filter);
									return;
								}
							}
						}
					}
				}
			}
		}
	}
    m_pCAN2EthernetDlg->AddInfomation ("Invalid Filter or Mask Value !!");
}

Boolean CTabSetup::isNumeric (char &ascii)
{
    if (ascii >= '0' && ascii <= '9')
    {
        ascii -= '0';
        return TRUE;
    }
    return FALSE;
}

Int8 CTabSetup::HexToDec (char code)
{
    if (code >= '0' && code <= '9')
    {
        return code - '0';
    }
    else if (code >= 'A' && code <= 'F')
    {
        return code - 'A' + 10;
    }
    return 0xff;
}

Boolean CTabSetup::getNumFromBuffer (char *buff, Int8 startPtr, Int32 &data, Int8 size)
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

void CTabSetup::SetupProcHandler (void)
{
    switch (m_setupProcMode)
	{
	case 0: // id 완료 
	    m_setupProcMode++;
		SetCANBaudRate ();
		break;

	case 1:	//can완료
        m_setupProcMode++;
	    SetBitTiming ();
		break;

	case 2:
		m_setupProcMode++;
		SetFilterMask ();
		break;
		
	case 3:
		m_pCAN2EthernetDlg->AddInfomation ("-- All Setup Complete --");
		break;
	}
	
}

void CTabSetup::SetBitTiming (void)
{
 	char buffer[20];
	Int32 data;
	Int8 brgcon[3];

	UpdateData (true);
	m_BRGCON1.MakeUpper();
	m_BRGCON2.MakeUpper();
	m_BRGCON3.MakeUpper();

	sprintf (buffer,"%s", m_BRGCON1);

    if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
	{
		brgcon[0] = (Int8)data;
		sprintf (buffer,"%s", m_BRGCON2);
        if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
		{
            brgcon[1] = (Int8)data;
            sprintf (buffer,"%s", m_BRGCON3);
			if (getNumFromBuffer (buffer,0, data, strlen(buffer)))
			{
				brgcon[2] = (Int8)data;
                m_pCAN2EthernetDlg->m_c2eConv.SetCANBitTiming (brgcon);
				return;
			}
		}
	}
    m_pCAN2EthernetDlg->AddInfomation ("Invalid BitTiming Value !!");
}

void CTabSetup::OnChkUserFilter() 
{
	Boolean userFlag = ((CButton*)GetDlgItem(IDC_CHK_USER_FILTER))->GetCheck() == 1;
	if (userFlag)
	{
        m_CAN_ID = "FFFFFFFE";		
	}
	((CEdit*)GetDlgItem(IDC_EDIT_CAN_ID))->EnableWindow(!userFlag);
	UpdateData (false);
}

void CTabSetup::OnBtnConnect() 
{
	UpdateData (true);

	if (!m_pCAN2EthernetDlg->m_c2eConv.m_isOnline)
	{
	    m_pCAN2EthernetDlg->AddInfomation ("Trying to connect .. Wait a moment !!");
	    m_pCAN2EthernetDlg->m_c2eConv.Initialize (AfxGetMainWnd()->m_hWnd, m_IP_Addr, m_portNo);
	    SetTimer (TIMER_ID_WAIT_CONNECTION, 100, NULL);
	}
	else
	{
        m_pCAN2EthernetDlg->m_c2eConv.Close();		
	    ((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Connect");
		((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (false);					

		m_pCAN2EthernetDlg->SetAllEnableTab (FALSE);
	}
}

BOOL CTabSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_cboCANBaudRate.InsertString (0,"125 kbps");
	m_cboCANBaudRate.InsertString (1,"250 kbps");
	m_cboCANBaudRate.InsertString (2,"500 kbps");
	m_cboCANBaudRate.InsertString (3,"1000 kbps");
	m_cboCANBaudRate.InsertString (4,"User");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTabSetup::SetDefaultValue (void)
{
	CCAN2RS232Conv *conv = NULL;
	conv = &m_pCAN2EthernetDlg->m_c2eConv;

    m_IP_Addr = "192.168.10.100";
    m_portNo = 54321;
    m_cboCANBaudRate.SetCurSel (conv->m_CAN_BaudRate);
	m_BRGCON1.Format ("%02X", conv->m_BRGCON[0]);
	m_BRGCON2.Format ("%02X", conv->m_BRGCON[1]);
	m_BRGCON3.Format ("%02X", conv->m_BRGCON[2]);
	m_CAN_ID.Format ("%08X", conv->m_CAN_ID);
	m_RX0FILTER0.Format ("%08X", conv->m_AccptanceFilter[0]);
	m_RX0FILTER1.Format ("%08X", conv->m_AccptanceFilter[1]);
	m_RX0MASK.Format ("%08X", conv->m_maskFilter[0]);
	m_RX1FILTER2.Format ("%08X", conv->m_AccptanceFilter[2]);
	m_RX1FILTER3.Format ("%08X", conv->m_AccptanceFilter[3]);
	m_RX1FILTER4.Format ("%08X", conv->m_AccptanceFilter[4]);
	m_RX1FILTER5.Format ("%08X", conv->m_AccptanceFilter[5]);
	m_RX1MASK.Format ("%08X", conv->m_maskFilter[1]);

	UpdateData (false);
}

void CTabSetup::OnTimer(UINT nIDEvent) 
{
    switch (nIDEvent)
	{
    case TIMER_ID_WAIT_CONNECTION:
		if (m_pCAN2EthernetDlg->m_c2eConv.m_isOnline)
		{
			KillTimer (TIMER_ID_WAIT_CONNECTION);
			m_pCAN2EthernetDlg->AddInfomation("Connected..");		
	    	((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Disconnect");
			((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (true);					
            m_pCAN2EthernetDlg->SetAllEnableTab (TRUE);
            
			SetTimer (TIMER_ID_WAIT_DISCONNECTION, 100, NULL);
		}
		break;
    case TIMER_ID_WAIT_DISCONNECTION:
		if (!m_pCAN2EthernetDlg->m_c2eConv.m_isOnline)
		{
			KillTimer (TIMER_ID_WAIT_DISCONNECTION);
			m_pCAN2EthernetDlg->AddInfomation("Disconnected..");		
	    	((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Connect");
			((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (false);					
            m_pCAN2EthernetDlg->SetAllEnableTab (FALSE);

			//CXTabCtrl::SelectTab(0);
			m_pCAN2EthernetDlg->m_menuTabCtrl.SelectTab (0);
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
