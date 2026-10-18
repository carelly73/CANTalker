// CAN2RS232Ppg.cpp : Implementation of the CCAN2RS232PropPage property page class.

#include "stdafx.h"
#include "CAN2RS232.h"
#include "CAN2RS232Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCAN2RS232PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCAN2RS232PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCAN2RS232PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCAN2RS232PropPage, "CAN2RS232.CAN2RS232PropPage.1",
	0xa9690998, 0x8acf, 0x4825, 0xb6, 0x3e, 0x32, 0x85, 0x40, 0x19, 0x66, 0xaa)


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232PropPage::CCAN2RS232PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCAN2RS232PropPage

BOOL CCAN2RS232PropPage::CCAN2RS232PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CAN2RS232_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232PropPage::CCAN2RS232PropPage - Constructor

CCAN2RS232PropPage::CCAN2RS232PropPage() :
	COlePropertyPage(IDD, IDS_CAN2RS232_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCAN2RS232PropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232PropPage::DoDataExchange - Moves data between page and properties

void CCAN2RS232PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCAN2RS232PropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCAN2RS232PropPage message handlers
