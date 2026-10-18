// vc_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vc_test.h"
#include "vc_testDlg.h"
#include "cantalker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVc_testDlg dialog

CVc_testDlg::CVc_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVc_testDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVc_testDlg)
	m_baudrate = 115200;
	m_port = 1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVc_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVc_testDlg)
	DDX_Control(pDX, IDC_CBO_INTF, m_cboIntf);
	DDX_Control(pDX, IDC_LIST_RCVCAN, m_lstCan);
	DDX_Text(pDX, IDC_EDIT_BAUDRATE, m_baudrate);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVc_testDlg, CDialog)
	//{{AFX_MSG_MAP(CVc_testDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_GET_INFO, OnBtnGetInfo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SEND_DATA, OnBtnSendData)
	ON_BN_CLICKED(IDC_BTN_CAN_INIT, OnBtnCanInit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVc_testDlg message handlers

BOOL CVc_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon	

    m_cboIntf.AddString ("RS232");
    m_cboIntf.AddString ("USB");
    m_cboIntf.SetCurSel (1);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVc_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVc_testDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVc_testDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVc_testDlg::OnBtnInit() 
{
	if (DNK_InitDLL() == ERR_OK)
	{
        UpdateData (TRUE);

        if (m_cboIntf.GetCurSel() == 0) // RS232
		{
			if (DNK_InitRS232Driver(m_port, m_baudrate) < ERR_OK)
			{
				goto Failed;
			}
			else
			{
				goto Success;
			}			
        }
		else
		{
			if (DNK_GetDeviceCount() > 0)
			{
				if (DNK_InitUSBDriver(0) < ERR_OK)
				{
					goto Failed;
				}
				else
				{
					goto Success;
				}
			}
			else
			{
				AfxMessageBox ("Device is not connected !!");
				DNK_CloseDLL ();
			}
		}	
	}
	else
	{
		AfxMessageBox ("DLL Initialization Failed !!");		
	}

	return;
		
Success:
    AfxMessageBox ("Initialization Complete");
	SetTimer (0, 50, NULL);
	return;
		
Failed:
    AfxMessageBox ("Driver Initialization Failed");
	DNK_CloseDLL ();
}

void CVc_testDlg::OnBtnGetInfo() 
{
    CString		str, tmp;
	UINT32		d32;
	UINT8		d8, d8_2;
	char		model[9], serial[9];
	int			ret;
	DEVICE_INFO devInf;
	CAN_STATUS	sts;
	FILTER_INFO fi;
	
	str = "";
	
	do 
	{
		if ((ret = DNK_ReqCanStatus (&sts)) < ERR_OK) 
		{	
			break;
		}
		str.Format ("CAN Status=%02X\r\nCAN ID=%08X\r\nBRGCON=%02X,%02X,%02X\r\n", 
			sts.status, sts.canId, sts.brgcon[0], sts.brgcon[1], sts.brgcon[2]);

		if ((ret = DNK_ReqErrorCount (&d8, &d8_2)) < ERR_OK) 
		{	
			break;
		}	
		tmp.Format ("TX Error=%d, Rx Error=%d\r\n", d8, d8_2);
		str += tmp;


		if ((ret = DNK_ReqDeviceInfo (&devInf)) < ERR_OK) 
		{	
			break;
		}
		memcpy (model, devInf.modelName, 8); model[8] = 0;
		memcpy (serial, devInf.serialNo, 8); serial[8] = 0;
		tmp.Format ("Model Name=%s, SerialNo=%s\r\n", model, serial);
		str += tmp;

		if ((ret = DNK_ReqRcvMode (&d32)) < ERR_OK) 
		{
			break;
		}
		tmp.Format ("Receive Mode=%d\r\n", d32);
		str += tmp;
		
		if ((ret = DNK_ReqCanFilters (&fi)) < ERR_OK)
		{
			break;
		}
		tmp.Format ("Mask=%08X, %08X\r\n", fi.mask[0], fi.mask[1]);
		str += tmp;
		tmp.Format ("Filter=%08X,%08X,%08X,%08X,%08X,%08X\r\n",
			fi.filter[0],fi.filter[0],fi.filter[1],fi.filter[2],fi.filter[3],fi.filter[4],fi.filter[5]);
		str += tmp;

		if ((ret = DNK_ReqVersion (&d8, &d8_2)) < ERR_OK)
		{
			break;
		}
		tmp.Format ("F/W Version=%d.%d\r\n", d8, d8_2);
		str += tmp;

		if ((ret = DNK_ReqSpeedInfo (&d32, &d8)) < ERR_OK)
		{
			break;
		}
		tmp.Format ("CAN Speed=%d, RS232 index=%d\r\n", d32, d8);
		str += tmp;

		if ((ret = DNK_ReqCurTimeStamp (&d32)) < ERR_OK)
		{
			break;
		}		
		tmp.FormatMessage ("Time Stamp,%d\r\n", d32);
		
		AfxMessageBox (str);
		return;
	}
	while (0);
	
	str.Format ("error code=%d", ret);
	AfxMessageBox  (str);	
}

void CVc_testDlg::OnClose() 
{
	KillTimer (0);
	DNK_CloseDriver ();
	DNK_CloseDLL ();	
	CDialog::OnClose();
}

void CVc_testDlg::OnBtnSendData() 
{
	CAN_FRAME can;
	can.canId = 0x123;
	can.dlc = 8;
	can.data[0] = 0x11;
	can.data[1] = 0x22;
	can.data[2] = 0x33;
	can.data[3] = 0x44;
	can.data[4] = 0x55;
	can.data[5] = 0x66;
	can.data[6] = 0x77;
	can.data[7] = 0x88;
	DNK_SendCanData (&can);
}

void CVc_testDlg::OnBtnCanInit() 
{
	DNK_SetCanInit ();	
}

void CVc_testDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0)
	{
		int cnt, i, ret;
		CAN_FRAME can;
		CString str;
		
		cnt = DNK_GetTotalRcvCanDataCount();
		if (cnt < ERR_OK)
		{	
			ret = cnt;
			goto Failed;
		}			
		if (cnt == 0)
		{
			goto Success;
		}
		
		for (i = 0; i < cnt; i++)
		{
			if ((ret = DNK_GetRcvCanData (&can)) < ERR_OK)
			{
				goto Failed;
			}			
		
			str.Format ("%08X %02d %02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",
				can.canId, can.dlc, can.data[0],can.data[1],can.data[2],can.data[3],can.data[4],can.data[5],can.data[6],can.data[7]);
			m_lstCan.AddString (str);
		}
		goto Success;

Failed:;
		AfxMessageBox (str);	

Success:;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CVc_testDlg::OnBtnClear() 
{
	m_lstCan.ResetContent ();	
}

void CVc_testDlg::OnBtnClose() 
{
	KillTimer (0);
	DNK_CloseDriver();
	DNK_CloseDLL();	
}
