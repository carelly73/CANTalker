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
#include "CAN2RS232Ctl.h"

CCAN2RS232Conv  *pC2RCov = NULL;
extern CCAN2RS232Ctrl *pC2RCtrl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAN2RS232Conv::CCAN2RS232Conv()
{
	pC2RCov = this;

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
}

CCAN2RS232Conv::~CCAN2RS232Conv()
{

}

void CCAN2RS232Conv::RxProc (void)
{        
    Int8  data = 0;
    Int32 fifoReadCount = 0;
    Int16 byteCounter = 0;
    Boolean bRcvedData = FALSE;
    
    fifoReadCount = m_serial.GetDataALL(fifoReadBuffer);
    for (byteCounter = 0; byteCounter < fifoReadCount; byteCounter++)
    {
        data = fifoReadBuffer[byteCounter];
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
        MainTask ();
    }
}


void CCAN2RS232Conv::MainTask (void)
{
    if (!m_bUartPortOpen) return;

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

void CCAN2RS232Conv::RcvDataProc (PacketData *packet_p)
{
	Int32 canID = *(Int32*)packet_p->data;
	Int8 length = packet_p->length - 4;

    pC2RCtrl->PacketReceived (canID, length, &m_pkBuffer[m_pkBufferTailPtr].data[4]);
}

void CCAN2RS232Conv::PacketInterpreter(PacketData *packet_p)
{
    if (!m_bUartPortOpen) return;
    
    switch (packet_p->pid)
    {
    case PID_CAN_RECEIVED_DATA:
		RcvDataProc (packet_p);
		break;

    case PID_CAN_RSP_STATUS:
		RspCanStatus (packet_p);
		break;

    case PID_PROTOCOL_ACK:
		m_bACK_signal = TRUE;	    
		pC2RCtrl->ProtocolAck (false, 0);
        break;

	case PID_PROTOCOL_NACK:
		m_bNACK_signal = TRUE;
		pC2RCtrl->ProtocolAck (true, packet_p->data[0]);
		break;

    default:
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
            return FALSE;
		}
    }
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
    if (!m_bUartPortOpen) return FALSE;
    
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

Boolean CCAN2RS232Conv::Initialize(Int8 port, Int32 brate)
{
	bool bSuccess = false;
	m_uartState = RS232_STATE_IDLE;	
    m_pkBufferTailPtr = 0;
    m_pkBufferHeadPtr = 0;

    if (!m_bUartPortOpen)
	{
		long tempbrate = brate;
		if (!isBaudRateCorrect(tempbrate))
		{
			return false;
		}
		
        bSuccess = m_serial.PortOpen(port,brate);
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

    if (CreatePacket (&packet, PID_CAN_SEND_DATA, len+4) == TRUE)
	{
		*(Int32*)packet.data = canID;
		memcpy (&packet.data[4], data, len);        
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);         
	}
}

void CCAN2RS232Conv::ReqCanStatus (void)
{
	PacketData packet;    

    CreatePacket (&packet, PID_CAN_REQ_STATUS, 0);
	SendPacketToUart (&packet);
}

void CCAN2RS232Conv::RspCanStatus (PacketData *packet_p)
{
    Int8 *data = packet_p->data;
	Int32 canID = *(Int32*)&data[1];
    pC2RCtrl->RspCanStatus ((short)data[0], canID, (short)data[5], (short)data[6], (short)data[7]);	
}

void CCAN2RS232Conv::SendPacketToUart(PacketData *packet_p)
{
    int        cnt;
	int        idx = 0;
	
    if (!m_bUartPortOpen) return;

	m_serial.WriteBYTE (packet_p->header); 
    m_serial.WriteBYTE (packet_p->pid); 
    m_serial.WriteBYTE (packet_p->length);

    for (cnt = 0; cnt < packet_p->length; cnt++)
    {
        m_serial.WriteBYTE (packet_p->data[cnt]); 
    }
    m_serial.WriteBYTE (packet_p->checksum);
    m_serial.WriteBYTE (packet_p->eot); 
}

BOOL CCAN2RS232Conv::SetCANID (Boolean extMode, Int32 id)
{
    PacketData packet;    

	if (!extMode)
	{
        if (id != 0xffffffff && id != 0xfffffffe && (id > 0x7ff || id < 0))
		{
			return false;
		}
	}
	m_bExtMode = extMode;
    if (CreatePacket (&packet, PID_CAN_SET_ID, sizeof(Int32)+1))
	{	
		packet.data[0] = extMode; // standard mode
		*(Int32*)(packet.data + 1) = id;

        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
    }
	return true;;
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
        SendPacketToUart (&packet);			
        DestroyPacket (&packet); 
		Sleep(10);
        m_serial.SetCommPort (rs232BaudRate[brate], 10);
		return;
    }
}

void CCAN2RS232Conv::SetCANBaudRate (CAN_BAUD_RATE brate)
{
    PacketData packet;    
	
    if (CreatePacket (&packet, PID_CAN_SET_BAUD_RATE, sizeof(Int8)))
	{	
		packet.data[0] = brate;
		
        MakeCheckSum (&packet);        
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		return;
    }
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
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		return;
    }
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
        SendPacketToUart (&packet);
        DestroyPacket (&packet);  
		return;
    }
}

BOOL CCAN2RS232Conv::isBaudRateCorrect(long &baudRate) 
{
	const Int32 brate[] = { 9600, 19200, 38400, 57600, 115200, 230400 };
    Int8  idx;
	Int8  sel = 0xff;

	for (idx = 0; idx < RS232_BAUD_RATE_TOTAL_NUM; idx++)
	{
        if (brate[idx] == baudRate)
		{
            sel = idx;
		}
	}
	if (sel < 0xff)
	{
		baudRate = sel;
		return true;
	}
	return false;
}
