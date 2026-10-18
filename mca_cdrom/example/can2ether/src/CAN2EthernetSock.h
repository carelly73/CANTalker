#if !defined(AFX_CAN2ETHERNETSOCK_H__0B0D6E6A_D297_4839_8477_ADA545F0E77D__INCLUDED_)
#define AFX_CAN2ETHERNETSOCK_H__0B0D6E6A_D297_4839_8477_ADA545F0E77D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAN2EthernetSock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCAN2EthernetSock command target
class CCAN2RS232Conv;

class CCAN2EthernetSock : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CCAN2EthernetSock();
	virtual ~CCAN2EthernetSock();

    CCAN2RS232Conv *m_conv;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAN2EthernetSock)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCAN2EthernetSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAN2ETHERNETSOCK_H__0B0D6E6A_D297_4839_8477_ADA545F0E77D__INCLUDED_)
