// CAN2RS232Conv.cpp: implementation of the CCAN2RS232Conv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "can2rs232.h"
#include "CAN2RS232Conv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "type.h"
#include "can2rs232dlg.h"

extern CCan2rs232Dlg *pMainDlg;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
            
CCAN2RS232Conv::CCAN2RS232Conv()
{
    m_bUartPortOpen   = false;
    m_pkBufferTailPtr = 0;
    m_pkBufferHeadPtr = 0;
	m_retryNoForSendPacket = 0;
    m_uartState = RS232_STATE_IDLE;

	memset (fifoReadBuffer, IN_BUFFER_SIZE, 0);
	
	for (int i = 0; i < TOTAL_PACKET_BUFFER_SIZE; i++)
	{
        m_pkBuffer[i].data = PNULL;
	}
	m_pkLastData.data = PNULL;

	m_CAN_ID = 0xffffffff;
	m_BRGCON[0] = m_BRGCON[1] = m_BRGCON[2] = 0;
	m_maskFilter[0] = m_maskFilter[1] = 0;
	m_AccptanceFilter[0] = m_AccptanceFilter[1] = m_AccptanceFilter[2] = 0;
    m_AccptanceFilter[5]= m_AccptanceFilter[3] = m_AccptanceFilter[4] = 0;

	m_CAN_BaudRate = 3;
	m_RS232_BaudRate = 4;
	m_port = 0;
	m_bACK_signal = FALSE;
	m_bNACK_signal = FALSE;
    m_hwnd = NULL;
}

CCAN2RS232Conv::~CCAN2RS232Conv()
{

}

void CCAN2RS232Conv::RxProc (BYTE *packet, UINT32 size)
{        
    Int8  data = 0;
    Int32 fifoReadCount = 0;
    Int16 byteCounter = 0;
    Boolean bRcvedData = FALSE;
    BYTE    *pData;

    if (packet) /* for USB */
    {
        fifoReadCount = size;
        pData = packet;
    }
    else
    {
        fifoReadCount = m_serial.GetDataALL(fifoReadBuffer);
        pData = fifoReadBuffer;
    }
    
    for (byteCounter = 0; byteCounter < fifoReadCount; byteCounter++)
    {
        data = pData[byteCounter];
        // pMainDlg->AddData(data); 
        switch (m_uartState)
        {
        case RS232_STATE_IDLE:
            if (PID_PROTOCOL_HEADER == data)
            {
                m_pkBuffer[m_pkBufferHeadPtr].header = data;
                m_uartState = RS232_STATE_RCV_PID;
            }
            break;
                        
        case RS232_STATE_RCV_PID:
            m_pkBuffer[m_pkBufferHeadPtr].pid= data;
            m_uartState = RS232_STATE_RCV_LENGTH;
            break;
            
        case RS232_STATE_RCV_LENGTH:
            m_pkBuffer[m_pkBufferHeadPtr].length= data;
            m_RxDataLength = data;
            m_RxDataPtr = 0;
            if (data > RS232_MAX_DATA_BUFFER_SIZE)
            {
                m_uartState = RS232_STATE_IDLE;
                break;
            }
            if (m_RxDataLength > 0)
            {
                if (m_pkBuffer[m_pkBufferHeadPtr].data != PNULL)
                {
					delete m_pkBuffer[m_pkBufferHeadPtr].data;
                    m_pkBuffer[m_pkBufferHeadPtr].data = PNULL;
                }
				m_pkBuffer[m_pkBufferHeadPtr].data = new Int8[m_RxDataLength];
                if (m_pkBuffer[m_pkBufferHeadPtr].data == PNULL)
                {
                    m_uartState = RS232_STATE_IDLE;
                }
                m_uartState = RS232_STATE_RCV_DATA;
            }
            else
            {
                m_uartState = RS232_STATE_RCV_CHECKSUM;
            }            
            break;
            
        case RS232_STATE_RCV_DATA:
            m_pkBuffer[m_pkBufferHeadPtr].data[m_RxDataPtr++] = data;
            if (m_RxDataPtr >= m_RxDataLength)
            {
                m_uartState = RS232_STATE_RCV_CHECKSUM;
            }
            break;
            
        case RS232_STATE_RCV_CHECKSUM:
            m_pkBuffer[m_pkBufferHeadPtr].checksum = data;
            m_uartState = RS232_STATE_RCV_EOT;
            break;
            
        case RS232_STATE_RCV_EOT:
            if (PID_PROTOCOL_EOT == data)
            {
                m_pkBuffer[m_pkBufferHeadPtr].eot = data;
                m_pkBufferHeadPtr = (m_pkBufferHeadPtr + 1) % TOTAL_PACKET_BUFFER_SIZE;
                bRcvedData = TRUE;
            }
            m_uartState = RS232_STATE_IDLE;
            break;

        default:
            m_uartState = RS232_STATE_IDLE;
            break;
        }
    }

    if (bRcvedData)
    {
        if (m_hwnd)
        {
            PostMessage (m_hwnd, WM_COMM_PK_DATA_READY, 0, 0);
        }
        else
        {
            PostMessage (pMainDlg->m_hWnd, WM_COMM_PK_DATA_READY, 0, 0);
        }
    }
}


void CCAN2RS232Conv::MainTask (void)
{
    if ((!m_bUartPortOpen && (pMainDlg && !pMainDlg->m_bUsbOpen)) || !pMainDlg ) return;

	if (m_pkBufferHeadPtr != m_pkBufferTailPtr)
    {
        while (m_pkBufferHeadPtr != m_pkBufferTailPtr)
        {
            if (IsPacketDataCorrect (&m_pkBuffer[m_pkBufferTailPtr]))
            {
                PacketInterpreter (&m_pkBuffer[m_pkBufferTailPtr]);
            }
            m_pkBufferTailPtr = (m_pkBufferTailPtr + 1) % TOTAL_PACKET_BUFFER_SIZE;
        }
    }
}

void CCAN2RS232Conv::DestroyPacket(PacketData *packet_p)
{
    if (packet_p->data != PNULL)
    {
		delete packet_p->data;
		packet_p->data = PNULL;
    }
    memset (packet_p, sizeof(PacketData), 0x00);
}

void CCAN2RS232Conv::PacketInterpreter(PacketData *packet_p)
{
    if ((!m_bUartPortOpen && (pMainDlg && !pMainDlg->m_bUsbOpen)) || !pMainDlg ) return;    
    
    switch (packet_p->pid)
    {
    case PID_RECEIVED_EXT_CAN_DATA:        
    case PID_RECEIVED_CAN_DATA:
        switch (pMainDlg->m_menuTabCtrl.GetCurSel())
        {
        case 1:
            pMainDlg->m_TabInfo.DispData (packet_p);
            break;
            
        case 2:
            pMainDlg->m_ObdMonitor.RcvDataProc (packet_p);
            break;

        case 3:
            pMainDlg->m_CanOpenAnal.RcvDataProc (packet_p);
        }				
		break;
        
    case PID_RSP_CAN_STATUS:
		RspCanStatus (packet_p);
		break;

    case PID_RSP_CAN_ERROR_COUNT:
        RspCanErrorCount (packet_p);
        break;

	case PID_RSP_CAN_FILTERS:
		RspCanFilters (packet_p);
		break;

    case PID_PROTOCOL_ACK:
		m_bACK_signal = TRUE;	    
        break;

	case PID_PROTOCOL_NACK:
		m_bNACK_signal = TRUE;
		break;

    default:
		pMainDlg->AddInfomation("Unknown command received");
        break;
    }
    DestroyPacket (packet_p);
}

Boolean CCAN2RS232Conv::IsPacketDataCorrect (PacketData *packet_p)
{
    if (packet_p->pid >= PID_START_ID && packet_p->pid <= PID_END_ID ||
        packet_p->pid == PID_PROTOCOL_ACK ||
        packet_p->pid == PID_PROTOCOL_NACK)
        
    {
        if (MakeCheckSumSub(packet_p) == packet_p->checksum)
		{
            return TRUE;
		}
        else
		{
			pMainDlg->AddInfomation("******* INVALID CHECKSUM (PID=%02X) *******", packet_p->pid);
            return FALSE;
		}
    }
	pMainDlg->AddInfomation("******* INVALID PID (PID=%02X) *******", packet_p->pid);
    return FALSE;
}

Int8 CCAN2RS232Conv::MakeCheckSumSub (PacketData *packet_p)
{
    unsigned char cnt, checksum;
	checksum = packet_p->pid;
    checksum += packet_p->length;
    for(cnt = 0; cnt < packet_p->length; cnt++)
        checksum += packet_p->data[cnt];
    return checksum;
}
    
void CCAN2RS232Conv::MakeCheckSum (PacketData *packet_p)
{
    packet_p->checksum = MakeCheckSumSub(packet_p);
}

Boolean CCAN2RS232Conv::CreatePacket (PacketData *packet_p, CAN2RS232_PID pid, Int8 length)
{
    if ((!m_bUartPortOpen && (pMainDlg && !pMainDlg->m_bUsbOpen)) || !pMainDlg ) return false;
    
    packet_p->header = PID_PROTOCOL_HEADER;
    packet_p->eot = PID_PROTOCOL_EOT;
    packet_p->data = PNULL;
    packet_p->pid = pid;
    packet_p->length = length;

    if (length > 0)
    {
		packet_p->data = PNULL;
		packet_p->data = new Int8[length];
        return packet_p->data != PNULL;
    }
    else
    {
        MakeCheckSum (packet_p);
    }
    return true;
}

Boolean CCAN2RS232Conv::Initialize(HWND hwnd)
{
	const Int32 speedArr[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800 };		
	bool  bSuccess = false;
	Int8  port;
	Int32 speed;

	m_hwnd      = hwnd;
	m_uartState = RS232_STATE_IDLE;	
    m_pkBufferTailPtr = 0;
    m_pkBufferHeadPtr = 0;

	m_serial.SetMessagedWnd(hwnd);

	pMainDlg->m_TabSetup.UpdateData (true);
	port = pMainDlg->m_TabSetup.m_cboPort.GetCurSel() + 1;
    speed = pMainDlg->m_TabSetup.m_cboRS232BaudRate.GetCurSel();

    if (!m_bUartPortOpen)
	{
        bSuccess = m_serial.PortOpen(port, speedArr[speed]);
		if (bSuccess)
		{
			m_serial.StartMonitoring();
			m_bUartPortOpen = true;
		}
	}	
	return bSuccess;
}

void CCAN2RS232Conv::Close (void)
{
	if (m_bUartPortOpen)
	{
		m_bUartPortOpen = false;
		m_serial.PortClose();
	}
}

void CCAN2RS232Conv::SendPacket (Int32 canID, Int8 len, Int8 *data)
{
	PacketData packet;    

    if (CreatePacket (&packet, PID_SEND_CAN_DATA, len+4) == TRUE)
	{
		*(Int32*)packet.data = canID;
		memcpy (&packet.data[4], data, len);        
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);         
	}
}
/*
void CCAN2RS232Conv::SendPacket (void)
{
	PacketData packet;    

    pMainDlg->AddInfomation("Send Packet");
	
    if (CreatePacket (&packet, PID_CAN_SEND_DATA, 8) == TRUE)
	{
        packet.data[0] = 0;
		packet.data[1] = 1;
		packet.data[2] = 2;
		packet.data[3] = 3;
		packet.data[4] = 4;
		packet.data[5] = 5;
		packet.data[6] = 6;
		packet.data[7] = 7;

        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);         

	    pMainDlg->AddInfomation("[RSP OK] Last Status");
	}
	else
	{
		pMainDlg->AddInfomation("[RSP ERROR] Last Status");
	}
}
*/

void CCAN2RS232Conv::ReqCanStatus (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_REQ_CAN_STATUS, 0);
	SendPacketToUart (&packet);
    pMainDlg->AddInfomation("Req CAN Status");
}

void CCAN2RS232Conv::ReqCanFilters (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_REQ_CAN_FILTERS, 0);
	SendPacketToUart (&packet);
    pMainDlg->AddInfomation("Req CAN Filters");
}

void CCAN2RS232Conv::RspCanFilters (PacketData *packet_p)
{
	Int32 *data = (Int32*)packet_p->data;

	pMainDlg->AddInfomation("Rsp CAN Filters");
	pMainDlg->AddInfomation("	-> MASK1 = %08X", data[0]);
    pMainDlg->AddInfomation("	-> MASK2 = %08X", data[1]); 
	pMainDlg->AddInfomation("	-> FILT1 = %08X", data[2]);
	pMainDlg->AddInfomation("	-> FILT2 = %08X", data[3]);
	pMainDlg->AddInfomation("	-> FILT3 = %08X", data[4]);
	pMainDlg->AddInfomation("	-> FILT4 = %08X", data[5]);
	pMainDlg->AddInfomation("	-> FILT5 = %08X", data[6]);
	pMainDlg->AddInfomation("	-> FILT6 = %08X", data[7]);
}

void CCAN2RS232Conv::RspCanStatus (PacketData *packet_p)
{
#define fOVERRUN                (1<<0)
#define fERRWARNING             (1<<1)
#define fRXWARNING              (1<<2)
#define fTXWARNING              (1<<3)
#define fRXBUSPASSIVE           (1<<4)
#define fTXBUSPASSIVE           (1<<5)
#define fTXBUSOFF               (1<<6)
#define fEXTIDMODE              (1<<7)

    Int8 *data = packet_p->data;
    pMainDlg->AddInfomation("Rsp CAN Status");
    pMainDlg->AddInfomation("  Status Code = %02X", data[0]);
    
    if (data[0] & fOVERRUN)     pMainDlg->AddInfomation("    -> OVERRUN\n");
    if (data[0] & fERRWARNING)  pMainDlg->AddInfomation("    -> Error Warning\n");
    if (data[0] & fRXWARNING)   pMainDlg->AddInfomation("    -> Rx Warning\n");
    if (data[0] & fTXWARNING)   pMainDlg->AddInfomation("    -> Tx Warning\n");
    if (data[0] & fRXBUSPASSIVE)pMainDlg->AddInfomation("    -> Rx Bus Passive\n");
    if (data[0] & fTXBUSPASSIVE)pMainDlg->AddInfomation("    -> Tx Bus Passive\n");
    if (data[0] & fTXBUSOFF)    pMainDlg->AddInfomation("    -> Bus Off\n");
    if (data[0] & fEXTIDMODE)   pMainDlg->AddInfomation("    -> Extend ID\n");
    
    
    pMainDlg->AddInfomation("  CAN ID = %04X", *(Int32*)&data[1]);
	pMainDlg->AddInfomation("  BRGCON[1-3] = %02X/%02X/%02X", data[5], data[6], data[7]);
}

void CCAN2RS232Conv::RspCanErrorCount (PacketData *packet_p)
{
    pMainDlg->AddInfomation("Rsp CAN Error Count");
    pMainDlg->AddInfomation("  Rx Error Count = %d\n", packet_p->data[0]);
    pMainDlg->AddInfomation("  Tx Error Count = %d\n", packet_p->data[1]);    
}

void CCAN2RS232Conv::SendPacketToUart(PacketData *packet_p)
{
    int         cnt;
	int         idx = 0;
    BYTE        buff[128];
	
    if ((!m_bUartPortOpen && (pMainDlg && !pMainDlg->m_bUsbOpen)) || !pMainDlg ) return;

    buff[idx++] = packet_p->header; 
    buff[idx++] = packet_p->pid; 
    buff[idx++] = packet_p->length;

    for (cnt = 0; cnt < packet_p->length; cnt++)
    {
        buff[idx++] = packet_p->data[cnt]; 
    }
    buff[idx++] = packet_p->checksum;
    buff[idx++] = packet_p->eot; 

    if (pMainDlg->m_bUsbOpen)
    {
        pMainDlg->m_mpUsbAPI.SendPacket (buff, (UINT32*)&idx, 1000);
    }
    else
    {
        m_serial.WriteData (buff, idx);
    }
}

void CCAN2RS232Conv::SetCANID (Boolean extMode, Int32 id)
{
    PacketData packet;    

    if (CreatePacket (&packet, PID_SET_CAN_ID, sizeof(Int32)+1))
	{	
		packet.data[0] = extMode; // standard mode
		*(Int32*)(packet.data + 1) = id;

        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET CAN ID=%08X", id);
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	pMainDlg->AddInfomation("Set CAN ID fail !!");

}

void CCAN2RS232Conv::SetRS232BaudRate (RS232_BAUD_RATE brate)
{
	const Int32 rs232BaudRate[] = {
		9600, 19200, 38400, 57600, 115200, 230400,460800
	};

    PacketData packet;    

    if (CreatePacket (&packet, PID_SET_RS232_BAUDRATE, sizeof(Int8)))
	{	
		packet.data[0] = brate;
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);			
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET RS232 Baudrate..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		Sleep(10);
        m_serial.SetCommPort (rs232BaudRate[brate], 10);
		return;
    }
	pMainDlg->AddInfomation("Set RS232 Baudrate fail !!");
}

void CCAN2RS232Conv::SetCANBaudRate (CAN_BAUD_RATE brate)
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_SET_CAN_BITRATE, sizeof(Int8)))
	{	
		packet.data[0] = brate;
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET CAN Baudrate..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	pMainDlg->AddInfomation("Set RS232 Baudrate fail !!");
}

void CCAN2RS232Conv::SetTimeStamp (BOOL on)
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_SET_CAN_RCV_MODE, 4))
	{	
		*(UINT32*)packet.data = on;
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET TimeStamp =%d\n", on);
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	pMainDlg->AddInfomation("Set RS232 Baudrate fail !!");
}

void CCAN2RS232Conv::ReqErrorCount (void)
{
    PacketData packet;    

    if (CreatePacket (&packet, PID_REQ_CAN_ERROR_COUNT, 0))
	{	
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("Req Error Count\n");
		return;
    }
	pMainDlg->AddInfomation("Set RS232 Baudrate fail !!");
}

    

void CCAN2RS232Conv::GeneralTimerProc(UINT id)
{
	switch (id)
	{
	case CV_TIMER_WAIT_ACK:
	    if (m_bACK_signal || m_bNACK_signal)
		{
			if (m_bACK_signal)
			{
		        pMainDlg->AddInfomation("  --> Setting Complete !!");
		        KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			}
			else if (m_bNACK_signal)
			{
                pMainDlg->AddInfomation("  --> Setting Error !!");
			}
			KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			PostMessage(m_hwnd,WM_CV_RSP_SIGNAL,m_bACK_signal,m_bNACK_signal);
			m_bACK_signal = m_bNACK_signal = 0;
		}

	    m_timerCnter++;

	    if (m_timerCnter >= 10 ) //  1ÃÊ ´ë±â 
		{
			m_bNACK_signal = TRUE;
			m_bACK_signal = FALSE;
            pMainDlg->AddInfomation(" --> Timeout !!");
		    KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			PostMessage(m_hwnd,WM_CV_RSP_SIGNAL,m_bACK_signal,m_bNACK_signal);
		}
		break;
	}
}

void CCAN2RS232Conv::WaitAckSignal (CV_TIMER_ID id)
{
	m_timerCnter = 0;
    SetTimer (m_hwnd, id, 100, NULL);
}

void CCAN2RS232Conv::SetCANBitTiming (Int8 brgcon[3])
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_SET_CAN_PARAM, sizeof(Int8)*3))
	{	
		packet.data[0] = brgcon[0];
		packet.data[1] = brgcon[1];
		packet.data[2] = brgcon[2];
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET CAN Bit Timing..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	pMainDlg->AddInfomation("SET CAN Bit Timing fail !!");
}

void CCAN2RS232Conv::SetFilters(Int32 Mask[2], Int32 Filter[6])
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_SET_CAN_FILTERS, sizeof(Int32)*(2+6)))
	{	
		*(Int32*)(packet.data + 0) =  Mask[0];
        *(Int32*)(packet.data + 4) =  Mask[1];
		*(Int32*)(packet.data + 8) =  Filter[0];
		*(Int32*)(packet.data + 12) =  Filter[1];
		*(Int32*)(packet.data + 16) =  Filter[2];
		*(Int32*)(packet.data + 20) =  Filter[3];
		*(Int32*)(packet.data + 24) =  Filter[4];
		*(Int32*)(packet.data + 28) =  Filter[5];		
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		pMainDlg->AddInfomation("SET Filter..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	pMainDlg->AddInfomation("SET Filter Fail !!");
}
