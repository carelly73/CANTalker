// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM_H__1AB652AF_812F_11D5_85EA_0004766ECA78__INCLUDED_)
#define AFX_COMM_H__1AB652AF_812F_11D5_85EA_0004766ECA78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IN_BUFFER_SIZE (4096*2)

class CComm  
{
protected:
	HANDLE	m_hComm;
	bool	m_bThreadFlag;
	HWND	m_hMessagedWnd;
	CWinThread *m_thread;
	BYTE	InQUE[IN_BUFFER_SIZE];
	static	UINT WatchCommThread(LPVOID lparam);
	
public:
	DWORD	m_EOS_position;
	DWORD	m_QueTail;
	BYTE	m_EOS;
	CComm();
	virtual ~CComm();

	bool	PortOpen(int port, int brate);
	void	PortClose(void);
	bool	SetCommPort(DWORD Baudrate, DWORD Timeout);
	DWORD 	WriteData(BYTE *data,int len);
	bool	WriteBYTE(BYTE data);
	void	StopMonitoring(void);
	void	StartMonitoring(void);
	void	SetEOS(BYTE bEOS);
	void	SetMessagedWnd(HWND hwnd);
	DWORD	GetDataALL(BYTE *data);
	DWORD	GetDataToEOS(BYTE *data);
	DWORD	isInEOS(void);
};

#endif // !defined(AFX_COMM_H__1AB652AF_812F_11D5_85EA_0004766ECA78__INCLUDED_)
