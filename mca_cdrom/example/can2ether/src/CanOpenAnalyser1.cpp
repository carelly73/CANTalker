// CanOpenAnalyser1.cpp : implementation file
//

#include "stdafx.h"
#include "can2ethernet.h"
#include "CanOpenAnalyser1.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalyser dialog
CCanOpenAnalyser	*pCanOpenAnal = NULL;

CCanOpenAnalyser::CCanOpenAnalyser(CWnd* pParent /*=NULL*/)
	: CDialog(CCanOpenAnalyser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCanOpenAnalyser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCanOpenAnalyser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanOpenAnalyser)
	DDX_Control(pDX, IDC_LIST_INFO, m_lstInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanOpenAnalyser, CDialog)
	//{{AFX_MSG_MAP(CCanOpenAnalyser)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_CHK_AUTOSCROLL, OnChkAutoscroll)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_ALIGN, OnChkAlign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalyser message handlers

ABORT_INFO  abortInfo[] =
{
    0x05030000, "Toggle bit not alternated",
    0x05040000, "SDO protocol timed out",
    0x05040001, "Client/Server command specifier not valid or unknown",
    0x05040002, "Invalid block size (Block Transfer mode only)",
    0x05040003, "Invalid sequence number (Block Transfer mode only)",
    0x05030004, "CRC error (Block Transfer mode only)",
    0x05030005, "Out of memory",
    0x06010000, "Unsupported access to an object",
    0x06010001, "Attempt to read a write-only object",
    0x06010002, "Attempt to write a read-only object",
    0x06020000, "Object does not exist in the Object Dictionary",
    0x06040041, "Object can not be mapped to the PDO",
    0x06040042, "The number and length of the objects to be mapped would exceed PDO length",
    0x06040043, "General parameter incompatibility reason",
    0x06040047, "General internal incompatibility in the device",
    0x06060000, "Object access failed due to a hardware error",
    0x06060010, "Data type does not match, lengh of service parameter does not match",
    0x06060012, "Data type does not match, lengh of service parameter is too high",
    0x06060013, "Data type does not match, lengh of service parameter is too low",
    0x06090011, "Sub-index does not exist",
    0x06090030, "Value range of parameter exceeded (only for write access)",
    0x06090031, "Value of parameter written too high",
    0x06090032, "Value of parameter written too low",
    0x06090036, "Maximum value is less than minimum value",
    0x08000000, "General error",
    0x08000020, "Data can not be transferred or stored to the application",
    0x08000021, "Data can not be transferred or stored to the application because of local control",
    0x08000022, "Data can not be transferred or stored to the application because of the present device state",
    0x08000023, "Object Dictionary dynamic generation fails or no Object Dictionary is present",
    0x0, NULL
};

EMER_INFO   emerInfo[] = 
{
    0x0000, 0xFF00, "Error Reset or No Error",
    0x1000, 0xFF00, "Generic Error",
    0x2000, 0xFF00, "Current",
    0x2100, 0xFF00, "current, device input side",
    0x2200, 0xFF00, "current, inside the device",
    0x2300, 0xFF00, "current, device output side",
    0x3000, 0xFF00, "Voltage",
    0x3100, 0xFF00, "mains voltage",
    0x3200, 0xFF00, "voltage inside the device",
    0x3300, 0xFF00, "output voltage",
    0x4000, 0xFF00, "Temperature",
    0x4100, 0xFF00, "ambient temperature",
    0x4200, 0xFF00, "device temperature",
    0x5000, 0xFF00, "Device hardware",
    0x6000, 0xFF00, "Device software",
    0x6100, 0xFF00, "internal software",
    0x6200, 0xFF00, "user software",
    0x6300, 0xFF00, "data set",
    0x7000, 0xFF00, "Additional modules",
    0x8000, 0xFF00, "Monitoring",
    0x8100, 0xFF00, "Communication",
    0x8110, 0xFFFF, "CAN overrun",
    0x8120, 0xFFFF, "Error Passive",
    0x8130, 0xFFFF, "Life Guard Error or Heartbeat Error",
    0x8140, 0xFFFF, "Recovered from Bus-Off",
    0x8200, 0xFF00, "Protocol Error",
    0x8210, 0xFFFF, "PDO not processed due to length error",
    0x8220, 0xFFFF, "Length exceeded",
    0x9000, 0xFF00, "External error",
    0xF000, 0xFF00, "Additional functions",
    0xFF00, 0xFF00, "Device specific",
    0,0,"Unknown"
};


void InitializeLinkedList (LINKED_LIST *List)
{
    InitializeCriticalSection (&List->cs);    
	List->Head = List->Tail = NULL;
}

BOOL IsLinkedListEmpty(LINKED_LIST *List)
{
    BOOL isEmpty;
    
    EnterCriticalSection (&List->cs);
    isEmpty = List->Head == NULL;
    LeaveCriticalSection (&List->cs);
    
	return isEmpty;
}

void InsertTailList (LINKED_LIST *List, PLIST_ITEM Entry)
{
    EnterCriticalSection (&List->cs);
	Entry->Next = NULL;
	if (List->Tail != NULL)
	{
	    Entry->Prev = List->Tail;
		List->Tail->Next = (Entry);
	}
	else
	{
		Entry->Prev = NULL;
		List->Head = Entry;
	}
	List->Tail = Entry;
    LeaveCriticalSection (&List->cs);
 }

PLIST_ITEM	RemoveHeadList (LINKED_LIST *List)
{
	PLIST_ITEM Entry;

    EnterCriticalSection (&List->cs);
    Entry = List->Head;

	if (List->Head)
	{
 		List->Head = List->Head->Next;

		if (List->Head == NULL)
		{
			List->Tail = NULL;
		}
		else
		{
			List->Head->Prev = NULL;
		}
	}
    LeaveCriticalSection (&List->cs);

    return Entry;
}

void CCanOpenAnalyser::EnQMsg (CAN_MSG *pCanMsg)
{    
	CAN_MSG_LIST *pMsg;

    pMsg = (CAN_MSG_LIST*)malloc (sizeof(CAN_MSG_LIST));
    if (pMsg)
    {        
        memcpy (&pMsg->msg, pCanMsg, sizeof(CAN_MSG));        
        InsertTailList (&m_canMsgList, (PLIST_ITEM)pMsg);
    }
}

BOOL CCanOpenAnalyser::DeQMsg (CAN_MSG *pCanMsg)
{
    CAN_MSG_LIST   *pMsg;

    if (IsLinkedListEmpty (&m_canMsgList))
    {
        return FALSE;
    }

    pMsg = (CAN_MSG_LIST*)RemoveHeadList (&m_canMsgList);
    memcpy (pCanMsg, &pMsg->msg, sizeof(CAN_MSG));        
    free (pMsg);

	return TRUE;
}

void CANOPEN_PRINT (int type, CAN_MSG *pMsg, char *str)
{
    const CXListBox::Color colorLst[] = 
        {CXListBox::Fuschia, CXListBox::Red, CXListBox::Yellow, CXListBox::Lime, CXListBox::White};  
     
    UINT32  i;
	char    buff[512], *pOutStr = buff;

	if (pCanOpenAnal == NULL)
	{
		return;
	}

    if (pCanOpenAnal->m_bAlign)
    {
        pOutStr += sprintf (pOutStr, "%8.3f, %03X, %-75s, [", (float)pMsg->timeStamp/1000.0, pMsg->canId, str);
    }
    else
    {
        pOutStr += sprintf (pOutStr, "%8.3f, %03X, %s, [", (float)pMsg->timeStamp/1000.0, pMsg->canId, str);
    }
    
    for (i = 0; i < pMsg->dlc; i++)
    {
        pOutStr += sprintf (pOutStr, "%02X ", pMsg->data[i]);
    }
	pOutStr += sprintf (pOutStr, "]");

	//pCanOpenAnal->m_lstInfo.AddString (buff);
	pCanOpenAnal->m_lstInfo.AddLine (colorLst[type], CXListBox::Black, buff);   
}

ERR_CODE CANopenHandlerNMT (CAN_MSG *pMsg)
{
    ERR_CODE    ret = ERR_NONE;
    char        outStr[128], *pOutStr = outStr;
    UINT8       CS = pMsg->data[0];
    UINT8       nodeId = pMsg->data[1];
    int         type = T_INFO1;

    pOutStr += sprintf (pOutStr, "[NMT] ");

    /* command specifier */
    switch (CS)
    {
    case 1:     pOutStr += sprintf (pOutStr, "Start Remote Node"); break;
    case 2:     pOutStr += sprintf (pOutStr, "Stop Remote Node"); break;
    case 128:   pOutStr += sprintf (pOutStr, "Enter Pre-operational State"); break;
    case 129:   pOutStr += sprintf (pOutStr, "Reset Node"); break;
    case 130:   pOutStr += sprintf (pOutStr, "Reset Communication"); break;
    default:
        type = T_ERROR;
        pOutStr += sprintf (pOutStr, "Unknown Service(CS=%d)", CS); 
        break;
    }

    pOutStr += sprintf (pOutStr, "(Node ID=%d)", nodeId); 
    CANOPEN_PRINT (type, pMsg, outStr);
    
    return ret;
}

ERR_CODE CANopenHandlerSYNC (CAN_MSG *pMsg)
{
    CANOPEN_PRINT (T_INFO2, pMsg, "[SYNC]");
	return ERR_NONE;
}
    
ERR_CODE CANopenHandlerEM (CAN_MSG *pMsg)
{
    ERR_CODE    ret = ERR_NONE;
    UINT16      errCode = *(UINT16*)&pMsg->data[0];
    UINT8       errReg = pMsg->data[2];    
    char        outStr[512], *pOutStr = outStr;
    UINT32      i;
    int         type = T_WARN;
    const char  *errTypeStr[] = 
    { 
        "generic", "current", "voltage", "temperature", 
        "communication", "device profile specific", "reserved", "manufactureer specific"
    };
    
    for (i = 0; emerInfo[i].errCode; i++)
    {
        if (emerInfo[i].errCode == (errCode & emerInfo[i].errMask))
        {
            break;
        }
    }
    
    pOutStr += sprintf (pOutStr, "[EMERGENCY] Error Code=%X(%s)/Error Registr=%X(", 
        errCode, emerInfo[i].meanStr, errReg);

    for (i = 0; i < 8; i++)
    {
        if (errReg & (1<<i))
        {
            pOutStr += sprintf (pOutStr, "%s ", errTypeStr[i]);
        }
    }
    sprintf (pOutStr, ")");

    CANOPEN_PRINT (type, pMsg, outStr);
    return ret;
}
    
ERR_CODE CANopenHandlerTS (CAN_MSG *pMsg)
{
    CANOPEN_PRINT (T_INFO2, pMsg, "[TIMESTAMP]");
	return ERR_NONE;
}
    
ERR_CODE CANopenHandlerPDO (CAN_MSG *pMsg, UINT8 no, UINT8 Tflag)
{
    char buff[128];
    sprintf (buff, "[%cPDO%d]", Tflag?'T':'R', no);
    CANOPEN_PRINT (T_NORMAL, pMsg, buff);
	return ERR_NONE;
}
    
ERR_CODE CANopenHandlerSDO (CAN_MSG *pMsg, UINT8 Tflag)
{
    ERR_CODE    ret = ERR_NONE;
    UINT8       sdoCmd = pMsg->data[0];
    UINT16      sdoCmdType = (sdoCmd & SDO_CMD_MASK) | (Tflag << 8);
    UINT16      objIndex = pMsg->data[1] | (pMsg->data[2] << 8);
    UINT8       subIndex = pMsg->data[3];
    UINT8       n, e, s, t, c;
	UINT32		len;
    UINT32      abortCode, i;
    const char  *trfStr[] = { "normal", "expedited" };
    char        outStr[256], *pOutStr = outStr;
    int         type = T_NORMAL;

    pOutStr += sprintf (pOutStr, "[%cSDO(%X %X)] ", Tflag?'T':'R', objIndex, subIndex);

    switch (sdoCmdType)
    {
    case SDO_CMD_INIT_DOWNLOAD_CS:
        n = (sdoCmd >> 2) & 0x03;
        e = (sdoCmd >> 1) & 0x01;
        s = (sdoCmd) & 0x01;
        len = *(UINT32*)&pMsg->data[4];
        if (e)
        {
            pOutStr += sprintf (pOutStr, "Download Req(%s)/size=%d/data=%X", trfStr[e], n, len);
        }
        else
        {
            pOutStr += sprintf (pOutStr, "Download Req(%s)/size=%d/next frame", trfStr[e], len);            
        }
        break;
        
    case SDO_CMD_INIT_DOWNLOAD_SC:
        pOutStr += sprintf (pOutStr, "Download Rsp");
        break;
        
    case SDO_CMD_DOWNLOAD_SEG_CS:
        t = (sdoCmd >> 4) & 0x01;
        n = (sdoCmd >> 1) & 0x07;
        c = (sdoCmd) & 0x01;
        pOutStr += sprintf (pOutStr, "Download Seg Req(Tog=%d)/size=%d/last seg=%d", t, n, c);
        break;
        
    case SDO_CMD_DOWNLOAD_SEG_SC:
        t = (sdoCmd >> 4) & 0x01;
        pOutStr += sprintf (pOutStr, "Download Seg Rsp(Tog=%d)", t);
        break;
        
    case SDO_CMD_INIT_UPLOAD_CS:
        pOutStr += sprintf (pOutStr, "Upload Req");
        break;
        
    case SDO_CMD_INIT_UPLOAD_SC:
        n = (sdoCmd >> 2) & 0x03;
        e = (sdoCmd >> 1) & 0x01;
        s = (sdoCmd) & 0x01;
        len = *(UINT32*)&pMsg->data[4];
        if (e)
        {
            pOutStr += sprintf (pOutStr, "Upload Rsp(%s)/size=%d/data=%X", trfStr[e], n, len);
        }
        else
        {
            pOutStr += sprintf (pOutStr, "Download Rsp(%s)/size=%d/next frame", trfStr[e], len);            
        }
        break;
        
    case SDO_CMD_UPLOAD_SEG_CS:
        t = (sdoCmd >> 4) & 0x01;
        pOutStr += sprintf (pOutStr, "Upload Segment Req(Tog=%d)", t);
        break;
        
    case SDO_CMD_UPLOAD_SEG_SC:
        t = (sdoCmd >> 4) & 0x01;
        n = (sdoCmd >> 1) & 0x07;
        c = (sdoCmd) & 0x01;
        pOutStr += sprintf (pOutStr, "Upload Segment Rsp(Tog=%d)/size=%d/last seg=%d", t, n, c);
        break;
        
    case SDO_CMD_ABORT_TRANSFER_CS:        
    case SDO_CMD_ABORT_TRANSFER_SC:
        type = T_WARN;
        abortCode = *(UINT32*)&pMsg->data[4];
        for (i = 0; abortInfo[i].abortCode; i++)
        {
            if (abortInfo[i].abortCode == abortCode)
            {
                pOutStr += sprintf (pOutStr, "Abort by %s(%X/%s)", 
					Tflag ? "Server":"Client", abortCode, abortInfo[i].abortDescription);
                break;
            }
        }
        if (abortInfo[i].abortCode == 0)
        {
            type = T_ERROR;
            pOutStr += sprintf (pOutStr, "Abort Unknown Code = %X", abortCode);
        }
        break;
        
    default:
        type = T_ERROR;
        pOutStr += sprintf (pOutStr, "Unknown SDO Command = %X", sdoCmd);
        break;
    }

    CANOPEN_PRINT (type, pMsg, outStr);
    return ret;
}
    
ERR_CODE CANopenHandlerEC (CAN_MSG *pMsg)
{
    ERR_CODE    ret = ERR_NONE;
    char        outStr[128], *pOutStr = outStr;

    pOutStr += sprintf (pOutStr, "[ERROR CONTROL] ");
    
    if (pMsg->dlc == 0)
    {
         pOutStr += sprintf (pOutStr, "Master Request");
    }
    else
    {
        UINT8 state = pMsg->data[0] & 0x7F;
        UINT8 toggle = pMsg->data[0] >> 7;

        pOutStr += sprintf (pOutStr, "(Tog=%d) ", toggle);
        
        switch (state)
        {
        case 0:     pOutStr += sprintf (pOutStr, "Initializing/Boot-up");  break;
        case 1:     pOutStr += sprintf (pOutStr, "Disconnected");  break;
        case 2:     pOutStr += sprintf (pOutStr, "Connecting");  break;
        case 3:     pOutStr += sprintf (pOutStr, "Preparing");  break;
        case 4:     pOutStr += sprintf (pOutStr, "Stopped");  break;
        case 5:     pOutStr += sprintf (pOutStr, "Operational");  break;
        case 127:   pOutStr += sprintf (pOutStr, "Pre-Operational");  break;        
        default:    pOutStr += sprintf (pOutStr, "Unknown state=%d", state);   break;
        }
    }
    
    CANOPEN_PRINT (T_WARN, pMsg, outStr);
    
    return ret;
}
    
ERR_CODE CANopenProtocolHandler (CAN_MSG *pMsg)
{
    UINT16      funcCode;
    UINT16      nodeId;
    ERR_CODE    ret = ERR_NONE;

    funcCode = pMsg->canId & COB_ID_FUNC_CODE_MASK;
    nodeId = pMsg->canId & COB_ID_NODE_IDMASK;

    switch (funcCode)
    {
    case FUNC_CODE_NMT_SERVICE:
        ret = CANopenHandlerNMT (pMsg);
        break;
        
    case FUNC_CODE_SYNC:     
    //case FUNC_CODE_EMERGENCY:		
		if (nodeId == 0)
		{
			ret = CANopenHandlerSYNC (pMsg);
		}
		else
		{
			ret = CANopenHandlerEM (pMsg);
		}
        break;   
        
    case FUNC_CODE_TIME_STAMP:
        ret = CANopenHandlerTS (pMsg);
        break;
        
    case FUNC_CODE_TPDO0:
    case FUNC_CODE_RPDO0:
    case FUNC_CODE_TPDO1:
    case FUNC_CODE_RPDO1:
    case FUNC_CODE_TPDO2:
    case FUNC_CODE_RPDO2:
    case FUNC_CODE_TPDO3:
    case FUNC_CODE_RPDO3:
        ret = CANopenHandlerPDO (pMsg, (UINT8)((funcCode-0x180)/0x100), (UINT8)((funcCode&0x0080)==0x0080));
        break;
        
    case FUNC_CODE_TSDO:
        ret = CANopenHandlerSDO (pMsg, 1);
        break;
        
    case FUNC_CODE_RSDO:
        ret = CANopenHandlerSDO (pMsg, 0);
        break;
        
    case FUNC_CODE_ERROR_CONTROL:
        ret = CANopenHandlerEC (pMsg);
        break;
        
    default:
        {
            char str[128];
            sprintf (str, "Invalid Function code = %x\n", funcCode);
            CANOPEN_PRINT (T_ERROR, pMsg, str);
        }
        break;
    }
    return ret;
}

void CCanOpenAnalyser::OnBtnClear() 
{		
#ifdef _SIMULATION_
    SetTimer (2, 10, NULL);
#endif
    m_lstInfo.ResetContent ();
}


BOOL CCanOpenAnalyser::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pCanOpenAnal    = this;
    m_baseTime      =   0;
	m_bAutoScroll   = TRUE;
    m_bAlign        = FALSE;
    
	 ((CButton*)GetDlgItem(IDC_CHK_AUTOSCROLL))->SetCheck (m_bAutoScroll);

	InitializeLinkedList (&m_canMsgList);

    m_lstInfo.SetHorizontalExtent (900);

    m_font.CreateFont(
       15,                        // nHeight
       0,                         // nWidth
       0,                         // nEscapement
       0,                         // nOrientation
       FW_NORMAL,                 // nWeight
       FALSE,                     // bItalic
       FALSE,                     // bUnderline
       0,                         // cStrikeOut
       ANSI_CHARSET,              // nCharSet
       OUT_DEFAULT_PRECIS,        // nOutPrecision
       CLIP_DEFAULT_PRECIS,       // nClipPrecision
       DEFAULT_QUALITY,           // nQuality
       DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
       "Courier New");                 // lpszFacename

    m_lstInfo.SetFont (&m_font);

	SetTimer (1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCanOpenAnalyser::OnChkAutoscroll() 
{
	m_bAutoScroll = ((CButton*)GetDlgItem(IDC_CHK_AUTOSCROLL))->GetCheck() == 1;	
}

#ifdef _SIMULATION_
#include "sample.h"
#endif

void CCanOpenAnalyser::OnBtnSave() 
{
    CListBox    *pLstBox = (CListBox*)GetDlgItem (IDC_LIST_INFO);  
    CString     str;        
    FILE        *fp;
    int         i;
    char        szFilter[] = "CANopen Log File(*.csv)|*.csv||";
    CTime       ti = CTime::GetCurrentTime();    
    CString     fname;

    fname.Format ("co_%02d%02d%02d.csv", ti.GetHour(), ti.GetMinute(),ti.GetSecond());
    
    CFileDialog fileDlg(FALSE, NULL, fname, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
    
    if(IDOK != fileDlg.DoModal())
    {
        return;
    }

    fp = fopen (fname, "w");
    
    if (!fp)
    {
        return;
    }

    fprintf (fp, "Time Stamp, COB-ID, Description, Raw Data\n");
    for (i = 0; i < pLstBox->GetCount(); i++)
    {
        char *cpstr;
        pLstBox->GetText (i, str);
        cpstr = (char*)(LPCTSTR)str;
        fprintf (fp, "%s\n", &cpstr[2]);
    }
    
    fclose (fp);
}

void CCanOpenAnalyser::UpdateCanMsg (void)
{
	CAN_MSG msg;

	if (IsLinkedListEmpty (&m_canMsgList))
	{
		return;
	}

	while (DeQMsg (&msg))
	{
		CANopenProtocolHandler (&msg);
	}

    if (m_bAutoScroll)
    {
	    m_lstInfo.SetTopIndex (m_lstInfo.GetCount()-1);
    }
}

static int idx = 0;
void CCanOpenAnalyser::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1:
		UpdateCanMsg ();
		break;

#ifdef _SIMULATION_
    case 2:
        {
            UINT32 i = 100;

            if (m_baseTime == 0)
            {
                m_baseTime = timeGetTime ();
            }
            while (i--)
            {
        		msg[idx].timeStamp = timeGetTime () - m_baseTime;
        		EnQMsg (&msg[idx]);		
                idx++;
                if (idx == sizeof(msg) / sizeof(CAN_MSG))
                {
                    idx = 0;
                    KillTimer (2);
                    break;
                }
            }
        }
        break;
#endif
	}

	CDialog::OnTimer(nIDEvent);
}

void CCanOpenAnalyser::RcvDataProc (PacketData *packet_p)
{
    UINT8   len, *data;
    CAN_MSG msg;
    UINT32  curTime = timeGetTime ();

    if (m_baseTime == 0)
    {
        m_baseTime = curTime;
    }

    len = packet_p->length - 4;
    data = packet_p->data + 4;

    memset (&msg, 0x00, sizeof(msg));

    msg.timeStamp   = curTime - m_baseTime;
    msg.canId       = *(UINT32*)(packet_p->data);
    msg.dlc         = len;
    if (len == 0)
    {
        msg.rtr = 1;
    }
    memcpy (msg.data, data, len);

    EnQMsg (&msg);
}

void CCanOpenAnalyser::OnChkAlign() 
{
	m_bAlign = ((CButton*)GetDlgItem(IDC_CHK_ALIGN))->GetCheck() == 1;	
}
