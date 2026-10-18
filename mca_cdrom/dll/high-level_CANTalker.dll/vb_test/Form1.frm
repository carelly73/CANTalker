VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "CANTalker Dll Test - DNK ICT"
   ClientHeight    =   8385
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   9480
   BeginProperty Font 
      Name            =   "Consolas"
      Size            =   9
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8385
   ScaleWidth      =   9480
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "RS232"
      Height          =   1215
      Left            =   120
      TabIndex        =   8
      Top             =   1320
      Width           =   2175
      Begin VB.TextBox txtRS232Baudrate 
         Height          =   315
         Left            =   1200
         TabIndex        =   12
         Text            =   "Text1"
         Top             =   720
         Width           =   735
      End
      Begin VB.TextBox txtRS232Port 
         Height          =   315
         Left            =   1200
         TabIndex        =   11
         Text            =   "Text1"
         Top             =   360
         Width           =   735
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         Caption         =   "Baudrate"
         Height          =   255
         Left            =   120
         TabIndex        =   10
         Top             =   720
         Width           =   975
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "Port"
         Height          =   255
         Left            =   120
         TabIndex        =   9
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.ComboBox cboInterface 
      Height          =   330
      ItemData        =   "Form1.frx":0000
      Left            =   120
      List            =   "Form1.frx":0002
      TabIndex        =   7
      Text            =   "Combo1"
      Top             =   840
      Width           =   1935
   End
   Begin VB.CommandButton cmdReqInfos 
      Caption         =   "(4) Req Info."
      Height          =   495
      Left            =   120
      TabIndex        =   5
      Top             =   4560
      Width           =   2055
   End
   Begin MSComctlLib.ListView lvRcvCanList 
      Height          =   7455
      Left            =   2400
      TabIndex        =   4
      Top             =   240
      Width           =   6975
      _ExtentX        =   12303
      _ExtentY        =   13150
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      HotTracking     =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   5
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "No"
         Object.Width           =   1235
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "Time Stamp"
         Object.Width           =   2293
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "ID"
         Object.Width           =   1058
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   "DLC"
         Object.Width           =   882
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Text            =   "DATA"
         Object.Width           =   5292
      EndProperty
   End
   Begin VB.CommandButton cmdListClear 
      Caption         =   "Clear"
      Height          =   375
      Left            =   8280
      TabIndex        =   3
      Top             =   7800
      Width           =   975
   End
   Begin VB.Timer timerCheckRcvData 
      Left            =   7200
      Top             =   7800
   End
   Begin VB.CommandButton cmdSendCanData 
      Caption         =   "(3) Send CAN Data"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   3960
      Width           =   2055
   End
   Begin VB.CommandButton cmdCanInit 
      Caption         =   "(2) CAN Initialize"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   3360
      Width           =   2055
   End
   Begin VB.CommandButton cmdInit 
      Caption         =   "(1) Initialize"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   2760
      Width           =   2055
   End
   Begin VB.Label Label1 
      Caption         =   "Interface"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   600
      Width           =   2055
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'
' D&K ICT
' http://www.dnkict.com
' 2014/03/22
'
' Main Test Form
' rev.0
'

Option Explicit

Private Sub cmdInit_Click()
    If DNK_InitDLL() = ERR_OK Then
        If cboInterface.ListIndex = 0 Then        'RS232
            If DNK_InitRS232Driver(txtRS232Port, txtRS232Baudrate) < ERR_OK Then
                GoTo Failed
            Else
                GoTo Success
            End If
        Else
            If DNK_GetDeviceCount() > 0 Then
                If DNK_InitUSBDriver(0) < ERR_OK Then
                    GoTo Failed
                Else
                    GoTo Success
                End If
            Else
                MsgBox "Device is not connected !!"
                Call DNK_CloseDLL
                End If
            End If
    Else
        MsgBox "DLL Initialization Failed !!"
    End If
    Exit Sub
    
Success:
    MsgBox "Initialization Complete"
    timerCheckRcvData.Interval = 100
    timerCheckRcvData.Enabled = True
    Exit Sub
    
Failed:
    MsgBox "Driver Initialization Failed"
    Call DNK_CloseDLL
    Exit Sub
End Sub

Private Sub cmdCanInit_Click()
    Call DNK_SetCanInit
End Sub

Private Sub cmdListClear_Click()
    Call lvRcvCanList.ListItems.Clear
End Sub

Function GetStringFromByteArray(d8() As Byte) As String
    Dim i As Integer
    Dim str As String
    
    str = ""
    
    For i = 0 To 7
        str = str & Chr$(d8(i))
    Next
    
    GetStringFromByteArray = str
End Function


Private Sub cmdReqInfos_Click()
    Dim str As String
    Dim d32 As Long
    Dim d8 As Byte, d8_2 As Byte
    Dim ret As Long
    Dim devInf As DEVICE_INFO
    Dim sts As CAN_STATUS
    Dim fi As FILTER_INFO
    
    str = ""
    
    ret = DNK_ReqCanStatus(sts)
    If ret < ERR_OK Then GoTo errtn
    str = str & "CAN Status=" & Hex(sts.status) & vbCrLf
    str = str & "CAN ID=" & Hex(sts.canID) & vbCrLf
    str = str & "BRGCON=" & Hex(sts.BRGCON(0)) & "," & Hex(sts.BRGCON(1)) & "," & Hex(sts.BRGCON(2)) & vbCrLf
    
    ret = DNK_ReqErrorCount(d8, d8_2)
    If ret < ERR_OK Then GoTo errtn
    str = str & "TX Error=" & d8 & " Rx Error=" & d8_2 & vbCrLf
    
    ret = DNK_ReqDeviceInfo(devInf)
    If ret < ERR_OK Then GoTo errtn
    str = str & "Model Name=" & GetStringFromByteArray(devInf.modelName) & vbCrLf
    str = str & "Serial No=" & GetStringFromByteArray(devInf.serialNo) & vbCrLf
    
    ret = DNK_ReqRcvMode(d32)
    If ret < ERR_OK Then GoTo errtn
    str = str & "Receive Mode=" & d32 & vbCrLf
    
    ret = DNK_ReqCanFilters(fi)
    If ret < ERR_OK Then GoTo errtn
    str = str & "Mask=" & Hex(fi.mask(0)) & "," & Hex(fi.mask(1)) & vbCrLf
    str = str & "Filter=" & Hex(fi.filter(0)) & "," & Hex(fi.filter(1)) & "," & Hex(fi.filter(2)) & _
            "," & Hex(fi.filter(3)) & "," & Hex(fi.filter(4)) & "," & Hex(fi.filter(5)) & vbCrLf
    
    ret = DNK_ReqVersion(d8, d8_2)
    If ret < ERR_OK Then GoTo errtn
    str = str & "F/W Version=" & d8 & "." & d8_2 & vbCrLf
    
    ret = DNK_ReqSpeedInfo(d32, d8)
    If ret < ERR_OK Then GoTo errtn
    str = str & "CAN Speed=" & d32 & "," & "RS232 index=" & d8 & vbCrLf
    
    ret = DNK_ReqCurTimeStamp(d32)
    If ret < ERR_OK Then GoTo errtn
    str = str & "Time Stamp=" & d32 & vbCrLf
    
    MsgBox str
    Exit Sub
errtn:
    MsgBox "error code=" & ret
End Sub

Private Sub cmdSendCanData_Click()
    Dim canFrame As CAN_FRAME
    
    canFrame.canID = &H123
    canFrame.dlc = 8
    canFrame.data(0) = &H0
    canFrame.data(1) = &H11
    canFrame.data(2) = &H22
    canFrame.data(3) = &H33
    canFrame.data(4) = &H44
    canFrame.data(5) = &H55
    canFrame.data(6) = &H66
    canFrame.data(7) = &H77
    
    Call DNK_SendCanData(canFrame)
    
End Sub



Private Sub Form_Load()
    Call cboInterface.AddItem("RS232")
    Call cboInterface.AddItem("USB")
    cboInterface.ListIndex = 0
    
    txtRS232Port = "4"
    txtRS232Baudrate = "115200"
End Sub

Private Sub Form_Unload(Cancel As Integer)
    timerCheckRcvData.Enabled = False
    Call DNK_CloseDLL
End Sub

Private Sub timerCheckRcvData_Timer()
    Dim cnt As Long
    Dim i As Long, j As Long
    Dim ret As Long
    Dim canFrame As CAN_FRAME
    Dim lstCnt As Long
    Dim str As String
    
    cnt = DNK_GetTotalRcvCanDataCount()
    If cnt < ERR_OK Then
        ret = cnt
        GoTo errrtn
    End If
    
    If cnt = 0 Then
        Exit Sub
    End If
    
    For i = 1 To cnt
        ret = DNK_GetRcvCanData(canFrame)
        If ret < ERR_OK Then
            GoTo errrtn
        End If
                
        lstCnt = lvRcvCanList.ListItems.Count() + 1
        Call lvRcvCanList.ListItems.Add(, , lstCnt)
        With lvRcvCanList.ListItems(lstCnt)
            .SubItems(1) = canFrame.TimeStamp
            .SubItems(2) = canFrame.canID
            .SubItems(3) = canFrame.dlc
            
            str = ""
            For j = 0 To canFrame.dlc - 1
                str = str & Right("0" & Hex(canFrame.data(j)), 2) & " "
            Next
            .SubItems(4) = str
        End With
    Next
    
    Exit Sub
    
errrtn:
    MsgBox "Error Code=" & ret
End Sub
