// can2rs232Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "can2rs232.h"
#include "can2rs232Dlg.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "type.h"

CCan2rs232Dlg *pMainDlg = NULL;

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
// CCan2rs232Dlg dialog

CCan2rs232Dlg::CCan2rs232Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCan2rs232Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCan2rs232Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCan2rs232Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCan2rs232Dlg)
	DDX_Control(pDX, IDC_LIST_INFORM, m_infoList);
	DDX_Control(pDX, IDC_MENU_TAB, m_menuTabCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCan2rs232Dlg, CDialog)
	//{{AFX_MSG_MAP(CCan2rs232Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLEAR_INFO, OnBtnClearInfo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENU_TAB, OnSelchangeMenuTab)
	ON_MESSAGE(WM_COMM_DATA_READY, OnCommDataReady)
    ON_MESSAGE(WM_COMM_PK_DATA_READY, OnCommPacketDataReady)
	ON_MESSAGE(WM_CV_RSP_SIGNAL, OnCVRspSignal)
    ON_MESSAGE(WM_USBAPI_RCV_DATA, OnUsbDataReceived)
    ON_MESSAGE(WM_DEVICE_REMOVED, OnDeviceRemoved)
	ON_NOTIFY(NM_CLICK, IDC_MENU_TAB, OnClickMenuTab)    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCan2rs232Dlg message handlers

BOOL CCan2rs232Dlg::OnInitDialog()
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

	pMainDlg = this;

    m_bUsbOpen = FALSE;
	
	m_TabSetup.Create(CTabSetup::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_TabSetup, "Setup", 0);
	m_TabSetup.m_pTab = &m_menuTabCtrl;

	m_TabInfo.Create(CTabInfo::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_TabInfo, "Monitor", 0);
	m_TabInfo.m_pTab = &m_menuTabCtrl;

	m_ObdMonitor.Create(COBDMonitor::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_ObdMonitor, "OBD-II Monitor", 0);
	m_ObdMonitor.m_pTab = &m_menuTabCtrl;    

	m_CanOpenAnal.Create(CCanOpenAnalyser::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_CanOpenAnal, "CANopen Analyser", 0);
	m_CanOpenAnal.m_pTab = &m_menuTabCtrl; 

	DWORD dwStyle = m_infoList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB;
	m_infoList.SetExtendedStyle (dwStyle);
	
	m_infoList.InsertColumn (1, "Informations",LVCFMT_LEFT,450);
	
	
	SetWindowText("CAN2RS232 Converter/Multi-CAN Analyzer Tester V2.4");
	m_menuTabCtrl.EnableTab(1, false);
    m_menuTabCtrl.EnableTab(2, false);
#ifndef _SIMULATION_
    m_menuTabCtrl.EnableTab(3, false);
#endif
	m_TabSetup.SetDefaultValue ();
    m_ObdMonitor.SetDefaultValue ();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCan2rs232Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCan2rs232Dlg::OnPaint() 
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
HCURSOR CCan2rs232Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCan2rs232Dlg::OnCommDataReady(WPARAM wp, LPARAM lp)
{
    m_converter.RxProc(NULL, 0);
}

void CCan2rs232Dlg::OnCommPacketDataReady(WPARAM wp, LPARAM lp)
{
    m_converter.MainTask();
}

void CCan2rs232Dlg::AddInfomation(char *fmt, ...)
{
	static int lineNo=0;
    va_list ap;
    va_start(ap, fmt);
	CString str;
	char buff[128];

	Int32 itemCnter;
		
    vsprintf (buff, fmt, ap);
    
	itemCnter = m_infoList.GetItemCount();
	m_infoList.InsertItem (itemCnter, buff, 0);
	m_infoList.EnsureVisible (itemCnter, TRUE);
}

static int preLen1, preLen2;

void CCan2rs232Dlg::OnTimer(UINT nIDEvent) 
{    
    switch (nIDEvent)
	{
	case CV_TIMER_WAIT_ACK:
		m_converter.GeneralTimerProc (nIDEvent);
        break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CCan2rs232Dlg::OnClose() 
{
    m_mpUsbAPI.ClosePipe();
    m_converter.Close();
	CDialog::OnClose();
}

void CCan2rs232Dlg::ClearInfomations(void)
{
	int total;
    
	total = m_infoList.GetItemCount ();
	while (total--)
	{
	    m_infoList.DeleteItem (total);
	}
}

void CCan2rs232Dlg::OnBtnClearInfo() 
{
	ClearInfomations ();
}


void CCan2rs232Dlg::OnCVRspSignal(WPARAM wparam, LPARAM lparam)
{
	if (wparam)
    m_TabSetup.m_ACK_NACK = 1;

	if (lparam)
    m_TabSetup.m_ACK_NACK = 2;
	m_TabSetup.SetupProcHandler ();
}

void CCan2rs232Dlg::OnSelchangeMenuTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CCan2rs232Dlg::OnClickMenuTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CCan2rs232Dlg::OnUsbDataReceived (WPARAM wparam, LPARAM lparam)
{
    BYTE    *data = (BYTE*)lparam;
    UINT32  size = (UINT32)wparam;
    BYTE    buff[2048];
    
    if (data)
    {
        memcpy (buff, data, size);
        free (data);

        m_converter.RxProc (buff, size);        
    }
}

void CCan2rs232Dlg::OnDeviceRemoved (WPARAM wparam, LPARAM lparam)
{

}

BOOL CCan2rs232Dlg::UsbDeviceOpen (void)
{
    do
    {
        m_bUsbOpen = FALSE;
        if (!m_mpUsbAPI.LoadMpUsbAPIDll(m_hWnd))
        {
            AfxMessageBox ("MPUsbAPI.Dll load error !!\n");
            break;
        }
        if (!m_mpUsbAPI.OpenPipe())
        {
            AfxMessageBox ("USB OpenPipe Error !!\n");
            break;
        }
        m_bUsbOpen = TRUE;
    }
    while (0);
    
    return m_bUsbOpen;
}
