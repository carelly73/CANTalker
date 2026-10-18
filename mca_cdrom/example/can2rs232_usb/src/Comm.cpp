// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Comm.h"
#include "type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComm::CComm()
{
	m_EOS = 0;
	m_hMessagedWnd = 0;
	m_hComm = 0;
	m_bThreadFlag = false;
	m_QueTail = 0;
	m_EOS_position = 0;
}

CComm::~CComm()
{
	m_bThreadFlag = false;
	PortClose();
}

UINT CComm::WatchCommThread(LPVOID lparam)
{

	DWORD	dwRead=0, dwNoOfRead;
	BYTE	buff[IN_BUFFER_SIZE/2];
	CComm	*pComm=(CComm*)lparam;
	CString str;

	while(pComm->m_bThreadFlag)
	{
		{
			ReadFile(pComm->m_hComm,buff,IN_BUFFER_SIZE/2,&dwNoOfRead,NULL);

            if (dwNoOfRead > 0)
			{
			    memcpy(pComm->InQUE+pComm->m_QueTail, buff, dwNoOfRead);
			
			    if(pComm->m_QueTail+dwNoOfRead > IN_BUFFER_SIZE/2)
				{
				    memmove(pComm->InQUE, pComm->InQUE+pComm->m_QueTail+dwNoOfRead-IN_BUFFER_SIZE/2, IN_BUFFER_SIZE/2);
				    pComm->m_QueTail = IN_BUFFER_SIZE/2;
				} 
				else 
				{
				    pComm->m_QueTail += dwNoOfRead;
				}
				
			    PostMessage(pComm->m_hMessagedWnd,WM_COMM_DATA_READY,0,0);
			}
		}
		Sleep(1);
	}

	return true;
}

bool CComm::PortOpen(int port, int brate)
{
	DWORD dwError;
	CString str;

	str.Format("\\\\.\\COM%d",port);
	
	m_hComm = CreateFile(str,GENERIC_READ | GENERIC_WRITE,0,	NULL, OPEN_EXISTING, 0, NULL);

   	if(m_hComm == INVALID_HANDLE_VALUE){
		dwError = GetLastError();
		AfxMessageBox("시리얼 포트가 이미 사용중이거나 열수가 없습니다. 다른포트를 설정하여주세요",MB_OK); 
	    return false;
	} 

	if (!SetCommPort(brate, 10))
	{        
		return false;
	}
	SetCommMask(m_hComm,EV_RXCHAR);
	SetupComm(m_hComm,IN_BUFFER_SIZE,IN_BUFFER_SIZE);
	PurgeComm(m_hComm,PURGE_TXABORT|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_RXCLEAR);
		
	return true;
	
}

void CComm::PortClose(void)
{
	if(m_hComm)
	{
		StopMonitoring();
        SetCommMask(m_hComm, 0);
        PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
		CloseHandle(m_hComm);
		m_hComm = 0;
	}
}

bool CComm::SetCommPort(DWORD Baudrate, DWORD Timeout)
{
	int fSuccess;
	COMMTIMEOUTS CommTimeOut;	

	DCB LdpDcb;

	LdpDcb.DCBlength = sizeof(LdpDcb);
	GetCommState(m_hComm,&LdpDcb);

	LdpDcb.BaudRate = Baudrate;
	LdpDcb.ByteSize = 8;
	LdpDcb.fDtrControl = DTR_CONTROL_DISABLE;
    LdpDcb.fRtsControl = RTS_CONTROL_DISABLE; 
    LdpDcb.StopBits = ONESTOPBIT;
    LdpDcb.Parity = NOPARITY;
	LdpDcb.fDsrSensitivity = 0;
    LdpDcb.fOutxCtsFlow = 0;
	LdpDcb.fOutxDsrFlow = 0;
	LdpDcb.fInX = 0;
	LdpDcb.fOutX = 0;

	
	fSuccess = SetCommState(m_hComm,&LdpDcb);		
		
	if (!fSuccess) 
	{
		//AfxMessageBox(" Error : Setting the ComPort for LDP ",MB_OK); 
		AfxMessageBox ("설정할수 없는 값으로 설정을 시도 했습니다. (예,속도 230400bps)");
		false;
	}
     
	CommTimeOut.ReadIntervalTimeout = 0;
	CommTimeOut.ReadTotalTimeoutMultiplier = 0;
	CommTimeOut.ReadTotalTimeoutConstant = Timeout;
    CommTimeOut.WriteTotalTimeoutMultiplier = 0;
	CommTimeOut.WriteTotalTimeoutConstant = 0;	
	
	fSuccess=SetCommTimeouts(m_hComm,&CommTimeOut);

	if(!fSuccess)
	{
//		AfxMessageBox(" Error : Setting the Timeout of the ComPort for LDP",MB_OK); 
		return false;
	}
	return true;
}

DWORD CComm::WriteData(BYTE *data,int len)
{
	DWORD dwBytesWritten;
	WriteFile(m_hComm,data,len,&dwBytesWritten,NULL);
	return dwBytesWritten;
}

bool CComm::WriteBYTE(BYTE data)
{
    return WriteData(&data,1) == 1;
}

void CComm::StopMonitoring(void)
{
	m_bThreadFlag = false;
}

void CComm::StartMonitoring(void)
{
	m_bThreadFlag = true;
	m_thread = AfxBeginThread(WatchCommThread,this,THREAD_PRIORITY_NORMAL);
}

void CComm::SetEOS(BYTE bEOS)
{
	m_EOS = bEOS;
}


void CComm::SetMessagedWnd(HWND hwnd)
{
	m_hMessagedWnd = hwnd;
}

DWORD CComm::GetDataALL(BYTE *data)
{
	DWORD dsize;

	memcpy(data,InQUE,m_QueTail);
	dsize = m_QueTail;
	m_QueTail=0;
	return dsize;
}

DWORD CComm::GetDataToEOS(BYTE *data)
{
	DWORD dsize;

	if(!isInEOS()) return 0;
	
	memcpy(data,InQUE,m_EOS_position);
	dsize = m_QueTail-m_EOS_position;
	memmove(InQUE, InQUE+m_EOS_position, dsize);
	m_QueTail -= m_EOS_position;

	return m_EOS_position;
}

DWORD CComm::isInEOS(void)
{
	DWORD i;
	
	if(!m_EOS) return 0;

	for(i=0;i<m_QueTail;i++){
		if(InQUE[i]==m_EOS){
			m_EOS_position = i+1;
			return i+1;
		}
	}
	return 0;
}
