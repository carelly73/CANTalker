// can2ethernetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "can2ethernet.h"
#include "can2ethernetDlg.h"
#include "type.h"

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
		//
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCan2ethernetDlg dialog

CCan2ethernetDlg::CCan2ethernetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCan2ethernetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCan2ethernetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCan2ethernetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCan2ethernetDlg)
	DDX_Control(pDX, IDC_MENU_TAB, m_menuTabCtrl);
	DDX_Control(pDX, IDC_LIST_INFORM, m_infoList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCan2ethernetDlg, CDialog)
	//{{AFX_MSG_MAP(CCan2ethernetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR_INFO, OnBtnClearInfo)
	ON_MESSAGE(WM_COMM_PK_DATA_READY, OnCommPacketDataReady)
	ON_MESSAGE(WM_CV_RSP_SIGNAL, OnCVRspSignal)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCan2ethernetDlg message handlers

BOOL CCan2ethernetDlg::OnInitDialog()
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
	
    m_c2eConv.m_pCAN2EthernetDlg = this;

    m_TabSetup.Create(CTabSetup::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_TabSetup, "Setup", 0);
	m_TabSetup.m_pTab = &m_menuTabCtrl;
    m_TabSetup.m_pCAN2EthernetDlg = this;

	m_TabInfo.Create(CTabInfo::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_TabInfo, "Monitor", 0);
	m_TabInfo.m_pTab = &m_menuTabCtrl;
    m_TabInfo.m_pCAN2EthernetDlg = this;

	m_CanOpenAnal.Create(CCanOpenAnalyser::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_CanOpenAnal, "CANopen Analyser", 0);
	m_CanOpenAnal.m_pTab = &m_menuTabCtrl; 

    m_multiSndMsg.Create(CMultiSendMsg::IDD, &m_menuTabCtrl);
	m_menuTabCtrl.AddTab(&m_multiSndMsg, "Multi Send Message", 0);
	m_multiSndMsg.m_pTab = &m_menuTabCtrl; 
    m_multiSndMsg.m_pCAN2EthernetDlg = this;

	DWORD dwStyle = m_infoList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB;
	m_infoList.SetExtendedStyle (dwStyle);
	
	m_infoList.InsertColumn (1, "Informations",LVCFMT_LEFT, 475);

    m_bRcvCanOpen = FALSE;
	
    SetAllEnableTab (FALSE);

    m_TabSetup.SetDefaultValue ();

    LoadINI ();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCan2ethernetDlg::SetAllEnableTab (BOOL enable)
{
    m_menuTabCtrl.EnableTab(1, enable);
#ifndef _SIMULATION_    
    m_menuTabCtrl.EnableTab(2, enable);
#endif
    m_menuTabCtrl.EnableTab(3, enable);
}
void CCan2ethernetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCan2ethernetDlg::OnPaint() 
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
HCURSOR CCan2ethernetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CCan2ethernetDlg::AddInfomation (char *fmt,...)
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

void CCan2ethernetDlg::OnCommPacketDataReady(WPARAM wp, LPARAM lp)
{
    m_c2eConv.MainTask();
}

void CCan2ethernetDlg::OnTimer(UINT nIDEvent) 
{
    switch (nIDEvent)
	{
	case CV_TIMER_WAIT_ACK:
		m_c2eConv.GeneralTimerProc (nIDEvent);
        break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CCan2ethernetDlg::OnCVRspSignal(WPARAM wparam, LPARAM lparam)
{
	if (wparam)
    m_TabSetup.m_ACK_NACK = 1;

	if (lparam)
    m_TabSetup.m_ACK_NACK = 2;
	m_TabSetup.SetupProcHandler ();
}

void CCan2ethernetDlg::OnBtnClearInfo() 
{
	int total;
    
	total = m_infoList.GetItemCount ();
	while (total--)
	{
	    m_infoList.DeleteItem (total);
	}	
}


void CCan2ethernetDlg::GetApplicationDir (CString &str)
{
    TCHAR szPath[_MAX_PATH];
    str ="";

    str = _T("");

    GetModuleFileName (AfxGetInstanceHandle(), szPath, _MAX_PATH);
    str = szPath;

    int nPos = str.ReverseFind('\\');
    if (nPos > 0)
    {
        str = str.Left(nPos+1);
    }
}

#define INI_FILENAME    "can2ether.ini"

void CCan2ethernetDlg::SaveINI (void)
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
    
    char tmpstr[256];
    CString appPath;    
    CString iniPath;
    CString data;
        
    GetApplicationDir (appPath);

    iniPath = appPath + INI_FILENAME;

    UINT32 i;

    for (i = 0; i < 10; i++)
    {        
        sprintf (tmpstr, "MSG%d USE", i+1);
        data =  (((CButton*)m_multiSndMsg.GetDlgItem(chkList[i]))->GetCheck () == 1) ? "1":"0";
        WritePrivateProfileString ("SEND MESSAGE", tmpstr, data, iniPath);       

        sprintf (tmpstr, "MSG%d", i+1);
        m_multiSndMsg.GetDlgItem(msgList[i])->GetWindowText (data);
        WritePrivateProfileString ("SEND MESSAGE", tmpstr, data, iniPath);
    }

}

void CCan2ethernetDlg::LoadINI (void)
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
    
    char data[256], tmpstr[256];
    CString appPath;    
    CString iniPath;
    int     temp;
        
    GetApplicationDir (appPath);

    iniPath = appPath + INI_FILENAME;

    UINT32 i;

    for (i = 0; i < 10; i++)
    {        
        sprintf (tmpstr, "MSG%d USE", i+1);
        GetPrivateProfileString ("SEND MESSAGE", tmpstr, "0", data, 256, iniPath); 
        temp = atoi(data);
        ((CButton*)m_multiSndMsg.GetDlgItem(chkList[i]))->SetCheck (temp);

        sprintf (tmpstr, "MSG%d", i+1);
        GetPrivateProfileString ("SEND MESSAGE", tmpstr, "", data, 256, iniPath);
        m_multiSndMsg.GetDlgItem(msgList[i])->SetWindowText (data);
    }
}

void CCan2ethernetDlg::OnClose() 
{
    SaveINI ();
	CDialog::OnClose();
}
