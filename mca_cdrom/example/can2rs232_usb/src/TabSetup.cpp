// TabSetup.cpp : implementation file
//

#include "stdafx.h"
#include "can2rs232.h"
#include "TabSetup.h"
#include "Can2rs232Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSetup dialog

extern CCan2rs232Dlg *pMainDlg;


CTabSetup::CTabSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CTabSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSetup)
	m_BRGCON1 = _T("");
	m_BRGCON2 = _T("");
	m_CAN_ID = _T("");
	m_RX0FILTER0 = _T("");
	m_RX0FILTER1 = _T("");
	m_RX0MASK = _T("");
	m_RX1FILTER2 = _T("");
	m_RX1FILTER3 = _T("");
	m_RX1FILTER4 = _T("");
	m_RX1FILTER5 = _T("");
	m_RX1MASK = _T("");
	m_BRGCON3 = _T("");
	//}}AFX_DATA_INIT
}


void CTabSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSetup)
	DDX_Control(pDX, IDC_CBO_PORT, m_cboPort);
	DDX_Control(pDX, IDC_CBO_RS232, m_cboRS232BaudRate);
	DDX_Control(pDX, IDC_CBO_CAN, m_cboCANBaudRate);
	DDX_Text(pDX, IDC_EDIT_BRGCON1, m_BRGCON1);
	DDV_MaxChars(pDX, m_BRGCON1, 2);
	DDX_Text(pDX, IDC_EDIT_BRGCON2, m_BRGCON2);
	DDV_MaxChars(pDX, m_BRGCON2, 2);
	DDX_Text(pDX, IDC_EDIT_CAN_ID, m_CAN_ID);
	DDV_MaxChars(pDX, m_CAN_ID, 8);
	DDX_Text(pDX, IDC_EDIT_RX0FILTER0, m_RX0FILTER0);
	DDX_Text(pDX, IDC_EDIT_RX0FILTER1, m_RX0FILTER1);
	DDX_Text(pDX, IDC_EDIT_RX0MASK, m_RX0MASK);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER2, m_RX1FILTER2);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER3, m_RX1FILTER3);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER4, m_RX1FILTER4);
	DDX_Text(pDX, IDC_EDIT_RX1FILTER5, m_RX1FILTER5);
	DDX_Text(pDX, IDC_EDIT_RX1MASK, m_RX1MASK);
	DDX_Text(pDX, IDC_EDIT_BRGCON3, m_BRGCON3);
	DDV_MaxChars(pDX, m_BRGCON3, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSetup, CDialog)
	//{{AFX_MSG_MAP(CTabSetup)
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_USER_FILTER, OnChkUserFilter)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_USB_CONNECT, OnBtnUsbConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSetup message handlers

BOOL CTabSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_cboCANBaudRate.InsertString (0,"125 kbps");
	m_cboCANBaudRate.InsertString (1,"250 kbps");
	m_cboCANBaudRate.InsertString (2,"500 kbps");
	m_cboCANBaudRate.InsertString (3,"1000 kbps");
	m_cboCANBaudRate.InsertString (4,"User");
    
	m_cboRS232BaudRate.InsertString(0, "9600 bps");
	m_cboRS232BaudRate.InsertString(1, "19200 bps");
	m_cboRS232BaudRate.InsertString(2, "38400 bps");
	m_cboRS232BaudRate.InsertString(3, "57600 bps");
	m_cboRS232BaudRate.InsertString(4, "115200 bps");
	m_cboRS232BaudRate.InsertString(5, "230400 bps");
	m_cboRS232BaudRate.InsertString(6, "460800 bps");

	CString str;
	Int8 idx;

	for (idx = 0; idx < 20; idx++)
	{
		str.Format("COM%d",idx + 1);
		m_cboPort.InsertString (idx, str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTabSetup::SetDefaultValue (void)
{
	CCAN2RS232Conv *conv = NULL;
	conv = &pMainDlg->m_converter;

    m_cboCANBaudRate.SetCurSel (conv->m_CAN_BaudRate);
	m_cboRS232BaudRate.SetCurSel (conv->m_RS232_BaudRate);
	m_cboPort.SetCurSel (conv->m_port);
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

void CTabSetup::OnBtnSetting() 
{
	const Int32 brate[]  = { 9600, 19200, 38400, 57600, 115200, 230400, 460800 };

    if (pMainDlg->m_bUsbOpen)
    {
	    m_setupProcMode = 1;
    }
    else
    {
        m_setupProcMode = 0;
    }
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
        pMainDlg->m_converter.SetCANID (extMode, data);	
	}
	else
	{
        pMainDlg->AddInfomation ("Invalid CAN ID=%s", buffer);
	}
}

void CTabSetup::SetRS232BaudRate (void)
{
	UpdateData (true);
	pMainDlg->m_converter.SetRS232BaudRate ((RS232_BAUD_RATE)m_cboRS232BaudRate.GetCurSel());
}

void CTabSetup::SetCANBaudRate (void)
{
	UpdateData (true);
	pMainDlg->m_converter.SetCANBaudRate ((CAN_BAUD_RATE)m_cboCANBaudRate.GetCurSel());
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
				                    pMainDlg->m_converter.SetFilters (mask, filter);
									return;
								}
							}
						}
					}
				}
			}
		}
	}
    pMainDlg->AddInfomation ("Invalid Filter or Mask Value !!");
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

void CTabSetup::SetupProcHandler (void)
{
    switch (m_setupProcMode)
	{
	case 0: // id 완료 
	    m_setupProcMode++;
		SetRS232BaudRate ();
		break;

    case 1: // rs232 완료
	    m_setupProcMode++;
		SetCANBaudRate ();
		break;

	case 2:	//can완료
        m_setupProcMode++;
	    SetBitTiming ();
		break;

	case 3:
		m_setupProcMode++;
		SetFilterMask ();
		break;
		
	case 4:
		pMainDlg->AddInfomation ("-- All Setup Complete --");
		break;

    case 10:
		const Int32 rs232BaudRate[] = {
		9600, 19200, 38400, 57600, 115200, 230400, 460800
		};

		if (m_ACK_NACK==1)
		{
            pMainDlg->AddInfomation("RS232 %ld bps Detected..!!", rs232BaudRate[m_detectSpeed]);
			return;
		}
		if (m_ACK_NACK==2)
		{
            m_detectSpeed++;
			if (m_detectSpeed < RS232_BAUD_RATE_TOTAL_NUM)
			{
		        pMainDlg->AddInfomation("Check %ld bps", rs232BaudRate[m_detectSpeed]);
                pMainDlg->m_converter.m_serial.SetCommPort(rs232BaudRate[m_detectSpeed],10);
	            pMainDlg->m_converter.SetRS232BaudRate ((RS232_BAUD_RATE)m_detectSpeed);
			}
			else
			{
                pMainDlg->AddInfomation("Device is not responded!!");
			}
		}
		
		break;
	}
	
}

void CTabSetup::OnTimer(UINT nIDEvent) 
{
    switch (nIDEvent)
	{
    case CV_TIMER_SETUP_PROC:
		break;
	}
	CDialog::OnTimer(nIDEvent);
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
                pMainDlg->m_converter.SetCANBitTiming (brgcon);
				return;
			}
		}
	}
    pMainDlg->AddInfomation ("Invalid BitTiming Value !!");
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

void CTabSetup::EnableControls (BOOL enable)
{
    ((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (enable);
    ((CButton*)GetDlgItem (IDC_BTN_USB_CONNECT))->EnableWindow (!enable);
    
    pMainDlg->m_menuTabCtrl.EnableTab(1, enable);
    pMainDlg->m_menuTabCtrl.EnableTab(2, enable);        
    pMainDlg->m_menuTabCtrl.EnableTab(3, enable);
}

void CTabSetup::OnBtnConnect() 
{
    if (pMainDlg->m_converter.m_bUartPortOpen)
	{
        pMainDlg->m_converter.Close ();
		((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Connect");
		((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (false);			
		pMainDlg->m_menuTabCtrl.EnableTab(1, false);
		pMainDlg->m_menuTabCtrl.EnableTab(2, false); 
        pMainDlg->m_menuTabCtrl.EnableTab(3, false); 
		return;
	}

    if (!pMainDlg->m_converter.Initialize(AfxGetMainWnd()->m_hWnd))
	{
        pMainDlg->AddInfomation("Initializing is error !!");
		((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Connect");
		((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (false);			
		pMainDlg->m_menuTabCtrl.EnableTab(1, false);
		pMainDlg->m_menuTabCtrl.EnableTab(2, false);
        pMainDlg->m_menuTabCtrl.EnableTab(3, false); 
        
	}
	else
	{
        pMainDlg->AddInfomation("Initializing is successful !!");		
		((CButton*)GetDlgItem (IDC_BTN_CONNECT))->SetWindowText("Disconnect");
		((CButton*)GetDlgItem (IDC_BTN_SETTING))->EnableWindow (true);
		pMainDlg->m_menuTabCtrl.EnableTab(1, true);
		pMainDlg->m_menuTabCtrl.EnableTab(2, true);
        pMainDlg->m_menuTabCtrl.EnableTab(3, true); 
        
	}	
}

void CTabSetup::OnBtnUsbConnect() 
{
    if (pMainDlg->UsbDeviceOpen ())
    {
        pMainDlg->m_converter.m_hwnd = AfxGetMainWnd()->m_hWnd;
        EnableControls (TRUE);
        ((CButton*)GetDlgItem (IDC_BTN_CONNECT))->EnableWindow (FALSE);
    }
}
