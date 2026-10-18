// MpUsbAPI.cpp: implementation of the CMpUsbAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MpUsbAPI.h"
#include "mpusb.h"
#include "Dbt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMpUsbAPI::CMpUsbAPI()
{
	m_hOutPipe	    = INVALID_HANDLE_VALUE;
	m_hInPipe	    = INVALID_HANDLE_VALUE;
	m_hLibHandle    = NULL;
    m_hThread       = NULL;
    m_rcvMsgHwnd    = NULL;
    m_bRunThread    = FALSE;
}

CMpUsbAPI::~CMpUsbAPI()
{
	ClosePipe ();
}

BOOL CMpUsbAPI::LoadMpUsbAPIDll (HWND hwnd)
{
    if (m_hLibHandle)
    {
        return TRUE;
    }
    
    m_hLibHandle = NULL;
    m_hLibHandle = LoadLibrary (_T("mpusbapi"));

    if(m_hLibHandle == NULL)
    {
        return FALSE;
    }
    else
    {
        MPUSBGetDLLVersion
            = (DWORD(*)(void))GetProcAddress (m_hLibHandle, "_MPUSBGetDLLVersion");
        
        MPUSBGetDeviceCount 
            = (DWORD(*)(PCHAR))GetProcAddress (m_hLibHandle, "_MPUSBGetDeviceCount");

        
        MPUSBOpen
            = (HANDLE(*)(DWORD,PCHAR,PCHAR,DWORD,DWORD))GetProcAddress (m_hLibHandle, "_MPUSBOpen");
        
        MPUSBWrite
            = (DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))GetProcAddress(m_hLibHandle, "_MPUSBWrite");
        
        MPUSBRead
            = (DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))GetProcAddress(m_hLibHandle, "_MPUSBRead");
        
        MPUSBReadInt
            = (DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))GetProcAddress(m_hLibHandle, "_MPUSBReadInt");
        
        MPUSBClose
            = (BOOL(*)(HANDLE))GetProcAddress(m_hLibHandle, "_MPUSBClose");

        if ((MPUSBGetDeviceCount == NULL) || 
            (MPUSBOpen == NULL) ||
            (MPUSBWrite == NULL) || 
            (MPUSBRead == NULL) ||
            (MPUSBClose == NULL) || 
            (MPUSBGetDLLVersion == NULL) ||
            (MPUSBReadInt == NULL))
        {
            return FALSE;
        }
    }

    m_rcvMsgHwnd = hwnd;

    return TRUE;
}

BOOL CMpUsbAPI::CheckDeviceRemoved (MSG *pMsg)
{
    if (pMsg->message == WM_DEVICECHANGE)
    {
        if((int)pMsg->wParam == DBT_DEVICEREMOVECOMPLETE)
        {
           if (!MPUSBGetDeviceCount (DEVICE_VID_PID))  
           {
               return TRUE;
           }
        }
    }
    return FALSE;
}

BOOL CMpUsbAPI::OpenPipe (void)
{
    GUID InterfaceClassGuid = {0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};
    
    //Register for WM_DEVICECHANGE notifications:
    static DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
    MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    MyDeviceBroadcastHeader.dbcc_reserved = 0;  //Reserved says not to use...
    MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
    RegisterDeviceNotification (m_rcvMsgHwnd, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);
    
    if (MPUSBGetDeviceCount (DEVICE_VID_PID))
    {
    	if (m_hOutPipe == INVALID_HANDLE_VALUE)
    	{
    		m_hOutPipe = MPUSBOpen (0, DEVICE_VID_PID, "\\MCHP_EP1", MP_WRITE,0);
    		if (m_hOutPipe == INVALID_HANDLE_VALUE)
    		{
    			return FALSE;
    		}
    	}
    	if (m_hInPipe == INVALID_HANDLE_VALUE)
    	{
    		m_hInPipe = MPUSBOpen (0, DEVICE_VID_PID, "\\MCHP_EP1", MP_READ,0);
    		if (m_hInPipe == INVALID_HANDLE_VALUE)
    		{              
    			MPUSBClose (m_hInPipe);
    			m_hInPipe = INVALID_HANDLE_VALUE;
    			return FALSE;
    		}
    	}
    }
    else
    {
        return FALSE;
    }

    m_bRunThread  = TRUE;
    m_hThread =  AfxBeginThread (MonitorThread, this,THREAD_PRIORITY_HIGHEST);
    m_hThread->m_bAutoDelete = FALSE;
	return TRUE;
}

void CMpUsbAPI::ClosePipe (void)
{
    if (m_bRunThread)
    {
        DWORD dwExitCode;
        
        m_bRunThread = FALSE;
        Sleep (100);
        
        GetExitCodeThread (m_hThread->m_hThread, &dwExitCode);
        if (dwExitCode == STILL_ACTIVE)
        {            
            TerminateThread (m_hThread->m_hThread, dwExitCode);
        }
    }
    
	if (m_hOutPipe != INVALID_HANDLE_VALUE)
	{
		MPUSBClose (m_hOutPipe);
		m_hOutPipe = INVALID_HANDLE_VALUE;
	}

	if (m_hInPipe != INVALID_HANDLE_VALUE)
	{
		MPUSBClose (m_hInPipe);
		m_hInPipe = INVALID_HANDLE_VALUE;
	}
}

BOOL CMpUsbAPI::SendPacket (BYTE *data, UINT32 *pSize, UINT32 timeOut)
{
    if (m_hOutPipe != INVALID_HANDLE_VALUE)
    {
        if (!MPUSBWrite (m_hOutPipe, data, *pSize, (DWORD*)pSize, timeOut))
        {
			CheckInvalidHandle ();
			return FALSE;
		}
		return TRUE;
    }
    return FALSE;
}


BOOL CMpUsbAPI::RcvPacket (BYTE *data, UINT32 *pSize, UINT32 timeOut)
{
    if (m_hInPipe != INVALID_HANDLE_VALUE)
    {
        if (!MPUSBRead (m_hInPipe, data, *pSize, (DWORD*)pSize, timeOut))
		{            
			CheckInvalidHandle ();
			return FALSE;
		}
    }
    return TRUE;
}

void CMpUsbAPI::CheckInvalidHandle (void)
{
	if (GetLastError () == ERROR_INVALID_HANDLE)
	{
        m_bRunThread = FALSE;
		MPUSBClose (m_hOutPipe);
		MPUSBClose (m_hInPipe);
		m_hOutPipe = m_hInPipe = INVALID_HANDLE_VALUE;
        PostMessage (m_rcvMsgHwnd, WM_DEVICE_REMOVED, 0, 0);
	}
}

UINT CMpUsbAPI::MonitorThread (LPVOID context)
{
    CMpUsbAPI *pUsbApi = (CMpUsbAPI*)context;
    BYTE    data[MAX_RCV_DATA_SIZE], *ptr;
    UINT32  size;

    while (pUsbApi->m_bRunThread)
    {
        /* if the size is not a unit of 1024 then bulk-in operation can be stopped */
        size = 4096;
        if (pUsbApi->RcvPacket (data, &size, 5000))
        {
            if (size && pUsbApi->m_rcvMsgHwnd)
            {
                #if 0                
                FILE *fp = fopen ("c:\\a.dat", "ab");                
                fwrite (data, size, 1, fp);
                fclose (fp);
                #endif                
                
                ptr = (BYTE*)malloc (size);
                if (ptr)
                {
                    memcpy (ptr, data, size);
                    PostMessage (pUsbApi->m_rcvMsgHwnd, WM_USBAPI_RCV_DATA, size, (LPARAM)ptr);                        
                }
            }
        }
    }

    
    return 0;
}

