; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGLSView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GLS.h"
LastPage=0

ClassCount=7
Class1=CGLSApp
Class2=CGLSDoc
Class3=CGLSView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME (English (U.S.))
Class5=CAboutDlg
Resource3=IDD_GLS_FORM
Resource4=IDD_DLG_MAP
Resource5=IDD_DLG_VARIAGRAM
Class6=CVariogram
Resource6=IDD_ABOUTBOX (English (U.S.))
Class7=CGridMap
Resource7=IDD_DLG_MAP (English (Australia))
Resource8=IDD_DLG_VARIAGRAM (English (Australia))
Resource9=IDD_GLS_FORM (English (U.S.))

[CLS:CGLSApp]
Type=0
HeaderFile=GLS.h
ImplementationFile=GLS.cpp
Filter=N

[CLS:CGLSDoc]
Type=0
HeaderFile=GLSDoc.h
ImplementationFile=GLSDoc.cpp
Filter=N

[CLS:CGLSView]
Type=0
HeaderFile=GLSView.h
ImplementationFile=GLSView.cpp
Filter=D
LastObject=CGLSView
BaseClass=CFormView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[CLS:CAboutDlg]
Type=0
HeaderFile=GLS.cpp
ImplementationFile=GLS.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_FILE_SAVE
Command6=ID_FILE_SAVE_AS
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command29=ID_VIEW_TOOLBAR
Command30=ID_VIEW_STATUS_BAR
Command32=ID_APP_ABOUT
CommandCount=32

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command17=ID_NEXT_PANE
Command18=ID_PREV_PANE
CommandCount=21

[DLG:IDD_GLS_FORM]
Type=1
Class=CGLSView

[DLG:IDD_GLS_FORM (English (U.S.))]
Type=1
Class=CGLSView
ControlCount=54
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_SRCPATH,edit,1350631552
Control4=IDC_EDIT_GRIDPATH,edit,1350631552
Control5=IDC_BTN_SRC,button,1342242816
Control6=IDC_BTN_GRID,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC_DATAVIEW,static,1342312449
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_NoCols,edit,1350631552
Control15=IDC_EDIT_XCol,edit,1350631552
Control16=IDC_EDIT_YCol,edit,1350631552
Control17=IDC_EDIT_DataCol,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_PARA1,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_PARA2,edit,1350631552
Control22=IDC_BTN_SEEDATA,button,1342242816
Control23=IDC_STATIC,button,1342177287
Control24=IDC_RADIO_GLOBAL,button,1342177289
Control25=IDC_RADIO_LOCAL,button,1342177289
Control26=IDC_EDIT_LOCALNUM,edit,1350631552
Control27=IDC_STATIC,static,1342308352
Control28=IDC_CHECK_XY,button,1342242819
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_EDIT_OUTPUT,edit,1350631552
Control34=IDC_EDIT_FILE,edit,1350631552
Control35=IDC_EDIT_FILERK,edit,1350631552
Control36=IDC_EDIT_FILEGLS,edit,1350631552
Control37=IDC_BTN_OUTPUT,button,1342242816
Control38=IDC_BTN_RUNKRIGING,button,1073807360
Control39=IDC_BTN_RUNRK,button,1073807360
Control40=IDC_BTN_RUNGLS,button,1342242816
Control41=IDC_BTN_VIEWOUTPUT,button,1073807360
Control42=IDC_BTN_MAP,button,1073807360
Control43=IDC_BTN_VARIOGRAM,button,1342242816
Control44=IDC_STATIC_PROGRESS,static,1342308352
Control45=IDC_STATIC,button,1342177287
Control46=IDC_STATIC,static,1342308352
Control47=IDC_EDIT_DISTANCE,edit,1350631552
Control48=IDC_STATIC_LOCAL,static,1342308352
Control49=IDC_STATIC,button,1342177287
Control50=IDC_COMBO_WEIGHT,combobox,1344339970
Control51=IDC_STATIC,button,1342177287
Control52=IDC_EDIT_ITERATE,edit,1350631552
Control53=IDC_STATIC,button,1342177287
Control54=IDC_COMBO_ESTIMATORS,combobox,1344339970

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_APP_ABOUT
CommandCount=2

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DLG_VARIAGRAM (English (Australia))]
Type=1
Class=CVariogram
ControlCount=1
Control1=IDOK,button,1342242817

[CLS:CVariogram]
Type=0
HeaderFile=Variogram.h
ImplementationFile=Variogram.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CVariogram

[DLG:IDD_DLG_MAP (English (Australia))]
Type=1
Class=CGridMap
ControlCount=1
Control1=IDOK,button,1342242817

[CLS:CGridMap]
Type=0
HeaderFile=GridMap.h
ImplementationFile=GridMap.cpp
BaseClass=CDialog
Filter=D
LastObject=CGridMap
VirtualFilter=dWC

[DLG:IDD_DLG_MAP]
Type=1
Class=?
ControlCount=1
Control1=IDOK,button,1342242817

[DLG:IDD_DLG_VARIAGRAM]
Type=1
Class=?
ControlCount=1
Control1=IDOK,button,1342242817

