Attribute VB_Name = "CanTalkerDll"
'
' D&K ICT
' http://www.dnkict.com
' 2014/03/22
'
' DLL function and data type Definition
' rev.0
'

Type TCPIP_INFO
    port As Integer
    ipMode As Byte
    ipAddr(0 To 3) As Byte
    subnetMask(0 To 3) As Byte
    gateway(0 To 3) As Byte
    macAddr(0 To 5) As Byte
End Type

Type CAN_FRAME
    TimeStamp As Long
    canID As Long
    dlc As Byte
    data(0 To 7) As Byte
End Type

Type DEVICE_INFO
    modelName(0 To 7) As Byte
    serialNo(0 To 7) As Byte
End Type

Type FILTER_INFO
    mask(0 To 1) As Long
    filter(0 To 5) As Long
End Type

Type BRGCON
    BRGCON(0 To 2) As Byte
End Type

Type CAN_STATUS
    canID As Long
    status As Byte
    BRGCON(0 To 2) As Byte
End Type

Public Const ERR_OK = 0
Public Const ERR_DLL_NOT_LOADED = -1
Public Const ERR_DLL_NOT_EXIST = -2
Public Const ERR_DLL_LOAD_FAILED = -3
Public Const ERR_DRIVER_LOAD_FAILED = -4
Public Const ERR_DRIVER_NOT_INITIALIZED = -5
Public Const ERR_DEVICE_DISCONNECTED = -6
Public Const ERR_INVALID_PARAMETER = -7
Public Const ERR_NOT_ENOUGH_MEMORY = -8
Public Const ERR_GENERAL_FAILURE = -9
Public Const ERR_TIMEOUT = -10
Public Const ERR_ALREADY_DRIVER_OPENED = -11

Public Declare Function DNK_InitDLL Lib "cantalker.dll" () As Long
Public Declare Function DNK_GetDeviceCount Lib "cantalker.dll" () As Long
Public Declare Function DNK_InitUSBDriver Lib "cantalker.dll" (ByVal DeviceInstance As Long) As Long
Public Declare Function DNK_InitRS232Driver Lib "cantalker.dll" (ByVal PortNo As Long, ByVal BaudRate As Long) As Long
Public Declare Function DNK_CloseDriver Lib "cantalker.dll" () As Long
Public Declare Function DNK_CloseDLL Lib "cantalker.dll" () As Long
Public Declare Function DNK_SetCanInit Lib "cantalker.dll" () As Long
Public Declare Function DNK_SendCanData Lib "cantalker.dll" (ByRef canFrame As CAN_FRAME) As Long
Public Declare Function DNK_GetTotalRcvCanDataCount Lib "cantalker.dll" () As Long
Public Declare Function DNK_ClearRcvCanaData Lib "cantalker.dll" () As Long
Public Declare Function DNK_GetRcvCanData Lib "cantalker.dll" (ByRef canFrame As CAN_FRAME) As Long
Public Declare Function DNK_SetClearCanError Lib "cantalker.dll" () As Long
Public Declare Function DNK_ReqCanStatus Lib "cantalker.dll" (ByRef status As CAN_STATUS) As Long
Public Declare Function DNK_ReqErrorCount Lib "cantalker.dll" (ByRef txErrCnt As Byte, ByRef rxErrCnt As Byte) As Long
Public Declare Function DNK_SetCANID Lib "cantalker.dll" (ByVal ExtMode As Byte, ByVal Id As Long) As Long
Public Declare Function DNK_SetCANBaudRate Lib "cantalker.dll" (ByVal BitRate As Long) As Long
Public Declare Function DNK_ReqDeviceInfo Lib "cantalker.dll" (ByRef deviceInfo As DEVICE_INFO) As Long
Public Declare Function DNK_SetRcvMode Lib "cantalker.dll" (ByVal Mode As Long) As Long
Public Declare Function DNK_ReqRcvMode Lib "cantalker.dll" (ByRef RcvMode As Long) As Long
Public Declare Function DNK_ReqCanFilters Lib "cantalker.dll" (ByRef FilterInfo As FILTER_INFO) As Long
Public Declare Function DNK_ReqVersion Lib "cantalker.dll" (ByRef major As Byte, ByRef minor As Byte) As Long
Public Declare Function DNK_ReqSpeedInfo Lib "cantalker.dll" (ByRef CanSpeed As Long, ByRef Rs232SpeedIdx As Byte) As Long
Public Declare Function DNK_ReqTcpIpInfo Lib "cantalker.dll" (ByRef TcpIpInfo As TCPIP_INFO) As Long
Public Declare Function DNK_ReqCurTimeStamp Lib "cantalker.dll" (ByRef TimeStamp As Long) As Long
Public Declare Function DNK_SetCANBitTiming Lib "cantalker.dll" (ByRef BRGCON As BRGCON) As Long
Public Declare Function DNK_SetFilters Lib "cantalker.dll" (ByRef FilterInfo As FILTER_INFO) As Long
Public Declare Function DNK_SetTcpIpInfo Lib "cantalker.dll" (ByRef TcpIpInfo As TCPIP_INFO) As Long
Public Declare Function DNK_SeRs232Baudrate Lib "cantalker.dll" (ByVal BaudRate As Byte) As Long
Public Declare Function DNK_CloseC2EConnection Lib "cantalker.dll" () As Long
