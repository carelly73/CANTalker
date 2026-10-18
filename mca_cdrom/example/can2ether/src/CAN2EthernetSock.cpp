// CAN2EthernetSock.cpp : implementation file
//

#include "stdafx.h"
#include "type.h"
#include "can2ethernet.h"
#include "CAN2EthernetSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCAN2EthernetSock
#include "CAN2EthernetDlg.h"
#include "CAN2RS232Conv.h"

CCAN2EthernetSock::CCAN2EthernetSock()
{
}

CCAN2EthernetSock::~CCAN2EthernetSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CCAN2EthernetSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CCAN2EthernetSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CCAN2EthernetSock member functions

void CCAN2EthernetSock::OnClose(int nErrorCode) 
{
    m_conv->m_isOnline = FALSE;
	Close ();
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CCAN2EthernetSock::OnConnect(int nErrorCode) 
{
	BOOL bTrue;
    
	SetSockOpt(TCP_NODELAY,&bTrue,sizeof(BOOL),IPPROTO_TCP);
	m_conv->m_isOnline = TRUE;
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CCAN2EthernetSock::OnReceive(int nErrorCode) 
{
    Int8 buff[1024];
	int nRead = Receive(buff, 1024);

    switch (nRead) 
	{
	case 0:
        m_conv->m_isOnline = FALSE;
		Close();
		break;

	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) {
			m_conv->m_isOnline = FALSE;
            Close();
        }
		break;

    default :
        m_conv->RxProc (buff, nRead);
        break;
    }
	
	CAsyncSocket::OnReceive(nErrorCode);
}
