// CAN2RS232Conv.cpp: implementation of the CCAN2RS232Conv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAN2RS232Conv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "type.h"
#include "CAN2EthernetDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAN2RS232Conv::CCAN2RS232Conv()
{
    m_isOnline   = false;
    m_pkBufferTailPtr = 0;
    m_pkBufferHeadPtr = 0;
	m_retryNoForSendPacket = 0;
    m_uartState = RS232_STATE_RCV_PID;
	
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
	m_port = 8088;
	m_bACK_signal = FALSE;
	m_bNACK_signal = FALSE;

	
    m_AckPacketReceived = 0;
    m_hwnd = 0;
	m_RxDataLength = 0;
	m_RxDataPtr = 0;
	m_timerCnter = 0;

    m_c2eSock.m_conv = this;
}

CCAN2RS232Conv::~CCAN2RS232Conv()
{

}

void CCAN2RS232Conv::RxProc (Int8 *fifoReadBuffer, int fifoReadCount)
{        
    Int8  data = 0;
    Int16 byteCounter = 0;
    Boolean bRcvedData = FALSE;
	Int8 checksum;
    
    for (byteCounter = 0; byteCounter < fifoReadCount; byteCounter++)
    {
        data = fifoReadBuffer[byteCounter];

        switch (m_uartState)
        {           
        case RS232_STATE_RCV_PID:
			m_pkBuffer[m_pkBufferHeadPtr].pid= data;
            m_uartState = RS232_STATE_RCV_LENGTH;
			checksum = data;
            break;
            
        case RS232_STATE_RCV_LENGTH:
            m_pkBuffer[m_pkBufferHeadPtr].length= data;
            m_RxDataLength = data;
            m_RxDataPtr = 0;
			checksum += data;
            if (data > RS232_MAX_DATA_BUFFER_SIZE)
            {
                m_uartState = RS232_STATE_RCV_PID;
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
                    m_uartState = RS232_STATE_RCV_PID;
                }
                m_uartState = RS232_STATE_RCV_DATA;
            }
            else
            {
                m_uartState = RS232_STATE_RCV_CHECKSUM;
            }            
            break;
            
        case RS232_STATE_RCV_DATA:
			checksum += data;
            m_pkBuffer[m_pkBufferHeadPtr].data[m_RxDataPtr++] = data;
            if (m_RxDataPtr >= m_RxDataLength)
            {
                m_uartState = RS232_STATE_RCV_CHECKSUM;
            }
            break;
            
        case RS232_STATE_RCV_CHECKSUM:
			if (checksum != data)
			{
				m_uartState = RS232_STATE_RCV_PID;
				break;
			}
            m_pkBuffer[m_pkBufferHeadPtr].checksum = data;    
            m_pkBufferHeadPtr = (m_pkBufferHeadPtr + 1) % TOTAL_PACKET_BUFFER_SIZE;
            bRcvedData = TRUE;
            m_uartState = RS232_STATE_RCV_PID;
            break;

        default:
            m_uartState = RS232_STATE_RCV_PID;
            break;
        }
    }

    if (bRcvedData)
    {
        SendMessage(m_hwnd,WM_COMM_PK_DATA_READY,0,0);
		MainTask ();
    }
}


void CCAN2RS232Conv::MainTask (void)
{
    if (!m_isOnline) return;

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
    if (!m_isOnline) return;
    
    switch (packet_p->pid)
    {
    case PID_CAN_RECEIVED_DATA:
        switch (m_pCAN2EthernetDlg->m_menuTabCtrl.GetCurSel())
        {
        case 1:
		    m_pCAN2EthernetDlg->m_TabInfo.DispData (packet_p);
		    break;
        case 2:
            m_pCAN2EthernetDlg->m_CanOpenAnal.RcvDataProc (packet_p);
            break;
        case 3:
            if (m_pCAN2EthernetDlg->m_bRcvCanOpen)
            {
                m_pCAN2EthernetDlg->m_CanOpenAnal.RcvDataProc (packet_p);
            }
            else
            {   
                m_pCAN2EthernetDlg->m_TabInfo.DispData (packet_p);
            }
            break;
        }
		break;

    case PID_CAN_RSP_STATUS:
		RspCanStatus (packet_p);
		break;

    case PID_RSP_VERSION:
		RspFirmwareVersion (packet_p);

    case PID_PROTOCOL_ACK:
		m_bACK_signal = TRUE;
        break;

	case PID_PROTOCOL_NACK:
		m_bNACK_signal = TRUE;
		break;

    default:
		m_pCAN2EthernetDlg->AddInfomation("Unknown command received=%02X", packet_p->pid);
        break;
    }
    DestroyPacket (packet_p);
}

Boolean CCAN2RS232Conv::IsPacketDataCorrect (PacketData *packet_p)
{
    if (packet_p->pid >= PID_START_ID && packet_p->pid <= PID_END_ID) 
    {
        if (MakeCheckSumSub(packet_p) == packet_p->checksum)
		{
            return TRUE;
		}
        else
		{
			m_pCAN2EthernetDlg->AddInfomation("******* INVALID CHECKSUM (PID=%02X) *******", packet_p->pid);
            return FALSE;
		}
    }
	m_pCAN2EthernetDlg->AddInfomation("******* INVALID PID (PID=%02X) *******", packet_p->pid);
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
    if (!m_isOnline) return FALSE;
    
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

void CCAN2RS232Conv::Initialize(HWND hwnd, CString ipaddr, Int16 port)
{
    m_c2eSock.Create();
	m_c2eSock.Connect (ipaddr, port);

	m_hwnd      = hwnd;
	m_uartState = RS232_STATE_RCV_PID;
    m_pkBufferTailPtr = 0;
    m_pkBufferHeadPtr = 0;

    m_isOnline = FALSE;
}

void CCAN2RS232Conv::Close (void)
{
	if (m_isOnline)
	{
        CloseConnection ();
	}
}

void CCAN2RS232Conv::CloseConnection (void)
{
	PacketData packet;      

    CreatePacket (&packet, PID_ETH_CLOSE_CONNECTION, 0);
	SendPacketToSocket (&packet);
    m_pCAN2EthernetDlg->AddInfomation("Req Close Connection");
}

void CCAN2RS232Conv::SendPacket (Int32 canID, Int8 len, Int8 *data)
{
	PacketData packet;    

    if (CreatePacket (&packet, PID_CAN_SEND_DATA, len+4) == TRUE)
	{
		*(Int32*)packet.data = canID;
		memcpy (&packet.data[4], data, len);        
        MakeCheckSum (&packet);        
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);         
	}
}
/*
void CCAN2RS232Conv::SendPacket (void)
{
	PacketData packet;    

    m_pCAN2EthernetDlg->AddInfomation("Send Packet");
	
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
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);         

	    m_pCAN2EthernetDlg->AddInfomation("[RSP OK] Last Status");
	}
	else
	{
		m_pCAN2EthernetDlg->AddInfomation("[RSP ERROR] Last Status");
	}
}
*/

void CCAN2RS232Conv::ReqCanStatus (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_CAN_REQ_STATUS, 0);
	SendPacketToSocket (&packet);
    m_pCAN2EthernetDlg->AddInfomation("Req CAN Status");
}

void CCAN2RS232Conv::ReqVersion (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_REQ_VERSION, 0);
	SendPacketToSocket (&packet);
    m_pCAN2EthernetDlg->AddInfomation("Req Firmware Version");
}

void CCAN2RS232Conv::RspCanStatus (PacketData *packet_p)
{
    Int8 *data = packet_p->data;
    m_pCAN2EthernetDlg->AddInfomation("Rsp CAN Status");
    m_pCAN2EthernetDlg->AddInfomation("  Status Code = %02X", data[0]);
    m_pCAN2EthernetDlg->AddInfomation("  CAN ID = %04X", *(Int32*)&data[1]);
	m_pCAN2EthernetDlg->AddInfomation("  BRGCON[1-3] = %02X/%02X/%02X", data[5], data[6], data[7]);
}

void CCAN2RS232Conv::CmdErrClear (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_CAN_CMD_CLEAR_ERROR, 0);
	SendPacketToSocket (&packet);
    m_pCAN2EthernetDlg->AddInfomation("Cmd Clear Error");
    WaitAckSignal (CV_TIMER_WAIT_ACK);
}

void CCAN2RS232Conv::RspFirmwareVersion (PacketData *packet_p)
{
    Int8 *data = packet_p->data;
    m_pCAN2EthernetDlg->AddInfomation("  Version = %d.%d", data[0], data[1]);
}

void CCAN2RS232Conv::SendPacketToSocket(PacketData *packet_p)
{
    int        cnt;
	int        idx = 0;
    Int8       buff[1024], len = 0;
	
    if (!m_isOnline) return;

    //buff[len++] = packet_p->header;
    buff[len++] = packet_p->pid;
    buff[len++] = packet_p->length;
    
    for (cnt = 0; cnt < packet_p->length; cnt++)
    {
        buff[len++] = packet_p->data[cnt];        
    }
    buff[len++] = packet_p->checksum;
    m_c2eSock.Send (buff, len);
}

void CCAN2RS232Conv::SetCANID (Boolean extMode, Int32 id)
{
    PacketData packet;    

    if (CreatePacket (&packet, PID_CAN_SET_ID, sizeof(Int32)+1))
	{	
		packet.data[0] = extMode; // standard mode
		*(Int32*)(packet.data + 1) = id;

        MakeCheckSum (&packet);        
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);  
		m_pCAN2EthernetDlg->AddInfomation("SET CAN ID=%08X", id);
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	m_pCAN2EthernetDlg->AddInfomation("Set CAN ID fail !!");

}

void CCAN2RS232Conv::SetRS232BaudRate (RS232_BAUD_RATE brate)
{
	const Int32 rs232BaudRate[] = {
		9600, 19200, 38400, 57600, 115200, 230400
	};

    PacketData packet;    

    if (CreatePacket (&packet, PID_RS232_SET_BAUD_RATE, sizeof(Int8)))
	{	
		packet.data[0] = brate;
		
        MakeCheckSum (&packet);        
        SendPacketToSocket (&packet);			
        DestroyPacket (&packet);  
		m_pCAN2EthernetDlg->AddInfomation("SET RS232 Baudrate..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		Sleep(10);
		return;
    }
	m_pCAN2EthernetDlg->AddInfomation("Set RS232 Baudrate fail !!");
}

void CCAN2RS232Conv::SetCANBaudRate (CAN_BAUD_RATE brate)
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_CAN_SET_BAUD_RATE, sizeof(Int8)))
	{	
		packet.data[0] = brate;
		
        MakeCheckSum (&packet);        
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);  
		m_pCAN2EthernetDlg->AddInfomation("SET CAN Baudrate..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	m_pCAN2EthernetDlg->AddInfomation("Set CAN Baudrate fail !!");
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
		        m_pCAN2EthernetDlg->AddInfomation("  --> Setting Complete !!");
		        KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			}
			else if (m_bNACK_signal)
			{
                m_pCAN2EthernetDlg->AddInfomation("  --> Setting Error !!");
			}
			KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			SendMessage(m_hwnd,WM_CV_RSP_SIGNAL,m_bACK_signal,m_bNACK_signal);
			m_bACK_signal = m_bNACK_signal = 0;
		}

	    m_timerCnter++;

	    if (m_timerCnter >= 10 ) //  1ÃÊ ´ë±â 
		{
			m_bNACK_signal = TRUE;
			m_bACK_signal = FALSE;
            m_pCAN2EthernetDlg->AddInfomation(" --> Timeout !!");
		    KillTimer (m_hwnd, CV_TIMER_WAIT_ACK);
			SendMessage(m_hwnd,WM_CV_RSP_SIGNAL,m_bACK_signal,m_bNACK_signal);
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
	
    if (CreatePacket (&packet, PID_CAN_SET_COMM_PARAM, sizeof(Int8)*3))
	{	
		packet.data[0] = brgcon[0];
		packet.data[1] = brgcon[1];
		packet.data[2] = brgcon[2];
		
        MakeCheckSum (&packet);        
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);  
		m_pCAN2EthernetDlg->AddInfomation("SET CAN Bit Timing..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	m_pCAN2EthernetDlg->AddInfomation("SET CAN Bit Timing fail !!");
}

void CCAN2RS232Conv::SetFilters(Int32 Mask[2], Int32 Filter[6])
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_CAN_FILTERS, sizeof(Int32)*(2+6)))
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
        SendPacketToSocket (&packet);
        DestroyPacket (&packet);  
		m_pCAN2EthernetDlg->AddInfomation("SET Filter..");
		WaitAckSignal (CV_TIMER_WAIT_ACK);
		return;
    }
	m_pCAN2EthernetDlg->AddInfomation("SET Filter Fail !!");
}