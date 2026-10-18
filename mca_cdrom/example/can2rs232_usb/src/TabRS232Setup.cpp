// TabRS232Setup.cpp : implementation file
//

#include "stdafx.h"
#include "can2rs232.h"
#include "TabRS232Setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "type.h"

/////////////////////////////////////////////////////////////////////////////
// CTabRS232Setup dialog


CTabRS232Setup::CTabRS232Setup(CWnd* pParent /*=NULL*/)
	: CDialog(CTabRS232Setup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRS232Setup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabRS232Setup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRS232Setup)
	DDX_Control(pDX, IDC_CBO_PORT, m_cboPort);
	DDX_Control(pDX, IDC_CBO_RS232, m_cboRS232BaudRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabRS232Setup, CDialog)
	//{{AFX_MSG_MAP(CTabRS232Setup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRS232Setup message handlers

BOOL CTabRS232Setup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cboRS232BaudRate.InsertString(0, "9600 bps");
	m_cboRS232BaudRate.InsertString(1, "19200 bps");
	m_cboRS232BaudRate.InsertString(2, "38400 bps");
	m_cboRS232BaudRate.InsertString(3, "57600 bps");
	m_cboRS232BaudRate.InsertString(4, "115200 bps");
	m_cboRS232BaudRate.InsertString(5, "230400 bps");

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
