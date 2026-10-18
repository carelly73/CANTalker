// OBDMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "can2rs232.h"
#include "OBDMonitor.h"
#include "Can2rs232Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COBDMonitor dialog

extern CCan2rs232Dlg *pMainDlg;

COBDMonitor::COBDMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(COBDMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(COBDMonitor)
	m_pollingInt = 0;
	//}}AFX_DATA_INIT
}


void COBDMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COBDMonitor)
	DDX_Control(pDX, IDC_LIST_MONITOR, m_monitorList);
	DDX_Text(pDX, IDC_EDT_POLL_INTV, m_pollingInt);
	DDV_MinMaxInt(pDX, m_pollingInt, 100, 5000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COBDMonitor, CDialog)
	//{{AFX_MSG_MAP(COBDMonitor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_OBD_MODE, OnBtnObdMode)
	ON_BN_CLICKED(IDC_CHK_ENGINE_LOAD, OnChkEngineLoad)
	ON_BN_CLICKED(IDC_CHK_VEHICLE_SPEED, OnChkVehicleSpeed)
	ON_BN_CLICKED(IDC_CHK_TIMING_ADVANCE, OnChkTimingAdvance)
	ON_BN_CLICKED(IDC_CHK_INTAKE_AIR_TEMP, OnChkIntakeAirTemp)
	ON_BN_CLICKED(IDC_CHK_MAF_AIR_FLOW_RATE, OnChkMafAirFlowRate)
	ON_BN_CLICKED(IDC_CHK_THROTTLE_POS, OnChkThrottlePos)
	ON_BN_CLICKED(IDC_CHK_ENGINE_TEMP, OnChkEngineTemp)
	ON_BN_CLICKED(IDC_CHK_FUEL_PRESSURE, OnChkFuelPressure)
	ON_BN_CLICKED(IDC_CHK_INTAKE_MAN_PRESSURE, OnChkIntakeManPressure)
	ON_BN_CLICKED(IDC_CHK_ENGINE_RPM, OnChkEngineRpm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COBDMonitor message handlers
// 
typedef enum
{
    RESERVED,
}
BLANK;

typedef struct
{
    char    *pPidName;
    char    *pUintStr;
    ULONG   ctlId;
    BOOL    enable;
    Int8    pidNo;
    Int8    datalen;
    void    (*dispcallback)(Int8 *data, Int8 len, char *dispBuff);
}
PID_INFO;

void dispCbEngineLoad (Int8 *data, Int8 len, char *dispBuff);
void dispCbEngineTemp (Int8 *data, Int8 len, char *dispBuff);
void dispCbFuelPress (Int8 *data, Int8 len, char *dispBuff);
void dispCbIntManPress (Int8 *data, Int8 len, char *dispBuff);
void dispCbEngineRpm (Int8 *data, Int8 len, char *dispBuff);
void dispCbVehSpeed (Int8 *data, Int8 len, char *dispBuff);
void dispCbTimingAdvan (Int8 *data, Int8 len, char *dispBuff);
void dispCbIntAirTemp (Int8 *data, Int8 len, char *dispBuff);
void dispCbMafAirFlowRate (Int8 *data, Int8 len, char *dispBuff);
void dispCbThPos (Int8 *data, Int8 len, char *dispBuff);


PID_INFO    pidInfList[] = 
{   
    "Engine Load",      "(%)",          IDC_CHK_ENGINE_LOAD,    TRUE,   0x04,   1,  dispCbEngineLoad,
    "Engine Temp",      "(deg C)",      IDC_CHK_ENGINE_TEMP,    TRUE,   0x05,   1,  dispCbEngineTemp,
    "Fuel Pressure",    "(kPa)",        IDC_CHK_FUEL_PRESSURE,  TRUE,   0x0A,   1,  dispCbFuelPress,
    "Intake Manifold Pressure","(kPa)", IDC_CHK_INTAKE_MAN_PRESSURE, TRUE, 0x0B, 1, dispCbIntManPress,
    "Engine RPM",       "(rpm)",        IDC_CHK_ENGINE_RPM,     TRUE,   0x0C,   2,  dispCbEngineRpm,
    "Vehicle Speed",    "(km/h)",       IDC_CHK_VEHICLE_SPEED,  TRUE,   0x0D,   1,  dispCbVehSpeed,
    "Timing Advance",   "(deg rel:1cyl)", IDC_CHK_TIMING_ADVANCE, TRUE, 0x0E, 1,  dispCbTimingAdvan,
    "Intake Air Temp",  "(deg C)",      IDC_CHK_INTAKE_AIR_TEMP, TRUE,0x0F,   1,  dispCbIntAirTemp,
    "MAF air flow rate", "(g/s)",       IDC_CHK_MAF_AIR_FLOW_RATE, TRUE, 0x10, 2, dispCbMafAirFlowRate,
    "Throttle Position","(%)",          IDC_CHK_THROTTLE_POS,   TRUE,   0x11,   1,  dispCbThPos,
    0x00, 
};

void dispCbEngineLoad (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data * 100 / 255);
}

void dispCbEngineTemp (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data - 40);
}

void dispCbFuelPress (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data * 3);
}

void dispCbIntManPress (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data);
}

void dispCbEngineRpm (Int8 *data, Int8 len, char *dispBuff)    
{
    sprintf (dispBuff, "%d", (*data * 256 + *(data+1)) / 4);
}

void dispCbVehSpeed (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data);
}

void dispCbTimingAdvan (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data / 2 - 64);
}

void dispCbIntAirTemp (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data - 40);
}

void dispCbMafAirFlowRate (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", (*data * 256 + *(data+1))/100);
}

void dispCbThPos (Int8 *data, Int8 len, char *dispBuff)
{
    sprintf (dispBuff, "%d", *data * 100 / 255);
}

void COBDMonitor::SetDefaultValue (void)
{
    PID_INFO    *pPidInfo = pidInfList;

    while (pPidInfo->pPidName)
    {
        CButton *pChkBox;
        CString str;
        
        pChkBox = (CButton*)GetDlgItem (pPidInfo->ctlId);
        
        pChkBox->SetCheck (pPidInfo->enable);

        m_monitorList.InsertItem (m_monitorList.GetItemCount(), pPidInfo->pPidName, 0);

        pPidInfo++;
    } 
}


BOOL COBDMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_monitorList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB;
	m_monitorList.SetExtendedStyle (dwStyle);
	
	m_monitorList.InsertColumn (1, "Name",LVCFMT_LEFT,150);
	m_monitorList.InsertColumn (2, "Value",LVCFMT_LEFT,100);	

	m_bMonitorStart = FALSE;
    m_pollingInt = 500;

    UpdateData (false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COBDMonitor::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == 1)
    {
        PID_INFO    *pPidInfo = pidInfList;
#if 0        
		PacketData packet;
		Int8 data[8];

		packet.pid = PID_CAN_RECEIVED_DATA;
		packet.data = data;
		*(UINT32*)data = 0x7E8;
		data[4] = 0x03;
		data[5] = 0x41;
		data[6] = 0x04;
		data[7] = rand();;

		pMainDlg->m_converter.PacketInterpreter (&packet);
#endif	

        while (pPidInfo->pPidName)
        {
            CButton *pChkBox;
            CString str;
            
            pChkBox = (CButton*)GetDlgItem (pPidInfo->ctlId);
            
            if (pChkBox->GetCheck ())
            {
                Int8 cmd[8];
                cmd[0] = 0x02;
                cmd[1] = 0x01;
                cmd[2] = pPidInfo->pidNo;
                cmd[3] = 0x55;
                cmd[4] = 0x55;
                cmd[5] = 0x55;
                cmd[6] = 0x55;
                cmd[7] = 0x55;
                pMainDlg->m_converter.SendPacket (0x7DF, 8, cmd);
            }
    
            pPidInfo++;
        }

    }
	
	CDialog::OnTimer(nIDEvent);
}

void COBDMonitor::OnBtnStart() 
{
	UpdateData (true);

	if (m_bMonitorStart)
	{
		KillTimer (1);
	}
	else
	{
		SetTimer (1, m_pollingInt, NULL);
	}

	m_bMonitorStart = !m_bMonitorStart;
}

void COBDMonitor::RcvDataProc (PacketData *packet_p)
{
	Int8 *data = &packet_p->data[4];
    Int8 len = data[0];
    Int8 mode = data[1] & 0x1F;
    Int8 pid = data[2];
    char dispStr[80];
    Int8 itemNo = 0;

    PID_INFO    *pPidInfo = pidInfList;
        
    while (pPidInfo->pPidName)
    {
        CButton *pChkBox;
           
        pChkBox = (CButton*)GetDlgItem (pPidInfo->ctlId);
        if (pChkBox->GetCheck ())
        {
            if (pPidInfo->pidNo == pid)
            {
                pPidInfo->dispcallback (&data[3], pPidInfo->datalen, dispStr);
                sprintf (dispStr, "%s %s", dispStr, pPidInfo->pUintStr);
                m_monitorList.SetItemText(itemNo, 1, dispStr);
                break;
            }
            itemNo++;
        }
        pPidInfo++;		
    }    
}

void COBDMonitor::OnBtnObdMode() 
{
	pMainDlg->m_converter.SetCANID (FALSE, 0x7E8);
	pMainDlg->m_converter.SetCANBaudRate (CAN_BAUD_RATE_500);

}

void COBDMonitor::OnChkEngineLoad() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkVehicleSpeed() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkTimingAdvance() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkIntakeAirTemp() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkMafAirFlowRate() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkThrottlePos() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkEngineTemp() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkFuelPressure() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkIntakeManPressure() 
{
	// TODO: Add your control notification handler code here
	
}

void COBDMonitor::OnChkEngineRpm() 
{
	// TODO: Add your control notification handler code here
	
}
