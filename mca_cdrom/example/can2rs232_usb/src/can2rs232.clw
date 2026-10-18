; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCanOpenAnalyser
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "can2rs232.h"

ClassCount=8
Class1=CCan2rs232App
Class2=CCan2rs232Dlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_DLG_OBD_SETUP
Resource2=IDR_MAINFRAME
Resource3=IDD_DLG_SETTING
Resource4=IDD_DLG_SETUP_RS232
Class4=CTabSetup
Class5=CTabInfo
Resource5=IDD_DLG_INFO_RCV_DATA
Class6=CTabRS232Setup
Resource6=IDD_ABOUTBOX
Resource7=IDD_DLG_OBD_MONITOR
Class7=COBDMonitor
Resource8=IDD_CAN2RS232_DIALOG
Class8=CCanOpenAnalyser
Resource9=IDD_DLG_CAN_OPEN

[CLS:CCan2rs232App]
Type=0
HeaderFile=can2rs232.h
ImplementationFile=can2rs232.cpp
Filter=N

[CLS:CCan2rs232Dlg]
Type=0
HeaderFile=can2rs232Dlg.h
ImplementationFile=can2rs232Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CCan2rs232Dlg

[CLS:CAboutDlg]
Type=0
HeaderFile=can2rs232Dlg.h
ImplementationFile=can2rs232Dlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CAN2RS232_DIALOG]
Type=1
Class=CCan2rs232Dlg
ControlCount=4
Control1=IDC_MENU_TAB,SysTabControl32,1342177280
Control2=IDC_STATIC,static,1342308352
Control3=IDC_LIST_INFORM,SysListView32,1350647813
Control4=IDC_BTN_CLEAR_INFO,button,1342242816

[DLG:IDD_DLG_INFO_RCV_DATA]
Type=1
Class=CTabInfo
ControlCount=31
Control1=IDC_BTN_REQ_STATUS,button,1342242816
Control2=IDC_LIST_CTRL,SysListView32,1350631425
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BTN_CLEAR_DATA,button,1342242816
Control5=IDC_BTN_REQ_AUTO_SEND,button,1342251008
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EDIT_SNDPK_ID,edit,1350631553
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_SNDPK_LEN,edit,1350631553
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_SNDPK_D0,edit,1350631553
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_SNDPK_D1,edit,1350631553
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_SNDPK_D2,edit,1350631553
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_SNDPK_D3,edit,1350631553
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_SNDPK_D4,edit,1350631553
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_SNDPK_D5,edit,1350631553
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_SNDPK_D6,edit,1350631553
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_SNDPK_D7,edit,1350631553
Control26=IDC_STATIC,static,1342308352
Control27=IDC_BTN_SND_PACKET,button,1342242816
Control28=IDC_CHK_TIME_STAMP,button,1342242819
Control29=IDC_BTN_REQ_ERROR_COUNT,button,1342251008
Control30=IDC_BTN_GET_COUNT,button,1342251008
Control31=IDC_BTN_REQ_REQ_CAN_FILTERS,button,1342251008

[DLG:IDD_DLG_SETTING]
Type=1
Class=CTabSetup
ControlCount=38
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_CAN_ID,edit,1350631552
Control3=IDC_BTN_SETTING,button,1476460544
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_BRGCON1,edit,1350631552
Control6=IDC_EDIT_BRGCON2,edit,1350631552
Control7=IDC_EDIT_BRGCON3,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_CBO_RS232,combobox,1344339970
Control15=IDC_CBO_CAN,combobox,1344339970
Control16=IDC_STATIC,button,1342177287
Control17=IDC_EDIT_RX0MASK,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_RX0FILTER0,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_RX0FILTER1,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_RX1MASK,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_RX1FILTER2,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_RX1FILTER3,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_RX1FILTER4,edit,1350631552
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_RX1FILTER5,edit,1350631552
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_CBO_PORT,combobox,1344339970
Control35=IDC_CHK_USER_FILTER,button,1342242819
Control36=IDC_CHK_EXT_ID_MODE,button,1342242819
Control37=IDC_BTN_CONNECT,button,1342242816
Control38=IDC_BTN_USB_CONNECT,button,1342242816

[CLS:CTabSetup]
Type=0
HeaderFile=TabSetup.h
ImplementationFile=TabSetup.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTabSetup

[CLS:CTabInfo]
Type=0
HeaderFile=TabInfo.h
ImplementationFile=TabInfo.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTabInfo

[DLG:IDD_DLG_SETUP_RS232]
Type=1
Class=CTabRS232Setup
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CBO_RS232,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CBO_PORT,combobox,1344339970
Control5=IDC_BTN_AUTODETECT,button,1342242816
Control6=IDC_BTN_AUTODETECT2,button,1342242816

[CLS:CTabRS232Setup]
Type=0
HeaderFile=TabRS232Setup.h
ImplementationFile=TabRS232Setup.cpp
BaseClass=CDialog
Filter=D
LastObject=CTabRS232Setup
VirtualFilter=dWC

[DLG:IDD_DLG_OBD_SETUP]
Type=1
Class=COBDMonitor
ControlCount=16
Control1=IDC_CHK_ENGINE_TEMP,button,1342242819
Control2=IDC_CHK_ENGINE_LOAD,button,1342242819
Control3=IDC_CHK_FUEL_PRESSURE,button,1342242819
Control4=IDC_CHK_INTAKE_MAN_PRESSURE,button,1342242819
Control5=IDC_CHK_ENGINE_RPM,button,1342242819
Control6=IDC_CHK_VEHICLE_SPEED,button,1342242819
Control7=IDC_CHK_TIMING_ADVANCE,button,1342242819
Control8=IDC_CHK_INTAKE_AIR_TEMP,button,1342242819
Control9=IDC_CHK_MAF_AIR_FLOW_RATE,button,1342242819
Control10=IDC_CHK_THROTTLE_POS,button,1342242819
Control11=IDC_STATIC,button,1342177287
Control12=IDC_LIST_MONITOR,SysListView32,1350631425
Control13=IDC_BTN_START,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDT_POLL_INTV,edit,1350631552
Control16=IDC_BTN_OBD_MODE,button,1342242816

[DLG:IDD_DLG_OBD_MONITOR]
Type=1
Class=?
ControlCount=0

[CLS:COBDMonitor]
Type=0
HeaderFile=OBDMonitor.h
ImplementationFile=OBDMonitor.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_DLG_CAN_OPEN]
Type=1
Class=CCanOpenAnalyser
ControlCount=5
Control1=IDC_LIST_INFO,listbox,1353777489
Control2=IDC_BTN_SAVE,button,1342242816
Control3=IDC_BTN_CLEAR,button,1342242816
Control4=IDC_CHK_AUTOSCROLL,button,1342242819
Control5=IDC_CHK_ALIGN,button,1342242819

[CLS:CCanOpenAnalyser]
Type=0
HeaderFile=CanOpenAnalyser.h
ImplementationFile=CanOpenAnalyser.cpp
BaseClass=CDialog
Filter=D
LastObject=CCanOpenAnalyser
VirtualFilter=dWC

