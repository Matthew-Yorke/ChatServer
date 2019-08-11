//*********************************************************************************************************************
//
// File Name: MainDialog.cpp
//
// Description:
//    TODO: Add description.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 08/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#include "stdafx.h"
#include "ChatServer.h"
#include "MainDialog.h"
#include "afxdialogex.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//*********************************************************************************************************************
//
// Function: MainDialog
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    thepParent - TODO: Add description.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
MainDialog::MainDialog(CWnd* thepParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, thepParent)
{
   mpDatabase = new Database();
   mpServer = new Server();
   mpServer->RegisterObserver(this);
	mHIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//***************************************************************************************************************
//
// Method: ~MainDialog
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//***************************************************************************************************************
MainDialog::~MainDialog()
{
   delete mpDatabase;
   mpDatabase = nullptr;
   delete mpServer;
   mpServer = nullptr;
}

//*********************************************************************************************************************
//
// Function: DoDataExchange
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    thepDataExchange - TODO: Add description.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void MainDialog::DoDataExchange(CDataExchange* thepDataExchange)
{
	CDialogEx::DoDataExchange(thepDataExchange);

   DDX_Text(thepDataExchange, IDC_HOST_EDIT_BOX, mDatabaseHost);
   DDX_Text(thepDataExchange, IDC_USER_EDIT_BOX, mDatabaseUser);
   DDX_Text(thepDataExchange, IDC_PASSWORD_EDIT_BOX, mDatabasePassword);
   DDX_Text(thepDataExchange, IDC_DATABASE_EDIT_BOX, mDatabaseName);
   DDX_Text(thepDataExchange, IDC_PORT_EDIT_BOX, mPortNumber);
}

//*********************************************************************************************************************
//
// MACRO: BEGIN_MESSAGE_MAP
//
// Description:
//    This macro maps callbacks for particular actions to functions calls.
//
// Arguments:
//    CChatServerDlg - TODO: Add description.
//    CDialogEx - TODO: Add description.
//
//*********************************************************************************************************************
BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_START_BUTTON, &MainDialog::OnStartButton)
END_MESSAGE_MAP()

//*********************************************************************************************************************
//
// Function: OnInitDialog
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    N/A
//
// Return:
//    TRUE - TODO: Add description.
//    FALSE - TODO: Add description.
//
//*********************************************************************************************************************
BOOL MainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != nullptr)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(mHIcon, TRUE);    // Set big icon
	SetIcon(mHIcon, FALSE);   // Set small icon

	return TRUE;  // return TRUE unless you set the focus to a control
}

//*********************************************************************************************************************
//
// Function: OnSysCommand
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    theID - TODO: Add description.
//    theParameter - TODO: Add description.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void MainDialog::OnSysCommand(UINT theID, LPARAM theParameter)
{
	if ((theID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(theID, theParameter);
	}
}

//*********************************************************************************************************************
//
// Function: OnPaint
//
// Description:
//    If you add a minimize button to your dialog, you will need the code below to draw the icon. For MFC applications
//    using the document/view model, this is automatically done for you by the framework.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void MainDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, mHIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//*********************************************************************************************************************
//
// Function: OnQueryDragIcon
//
// Description:
//    The system calls this function to obtain the cursor to display while the user drags the minimized windows.
//
// Arguments:
//    N/A
//
// Return:
//    TODO: Add description.
//
//*********************************************************************************************************************
HCURSOR MainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(mHIcon);
}

//*********************************************************************************************************************
//
// Function: OnStartButton
//
// Description:
//    TODO: Add description.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void MainDialog::OnStartButton()
{
   if(UpdateData(TRUE) == TRUE)
   {
      bool databaseConnection = false;
      std::string host = CStringA(mDatabaseHost);
      std::string user = CStringA(mDatabaseUser);
      std::string password = CStringA(mDatabasePassword);
      std::string databaseName = CStringA(mDatabaseName);

      databaseConnection = mpDatabase->Connect(host, mPortNumber, user, password, databaseName);

      if (databaseConnection == false)
      {
         AfxMessageBox(_T("Failed to connect to the database."), MB_OK | MB_ICONERROR);
      }
      else
      {
         AfxMessageBox(_T("Successfully connected to the database."), MB_OK | MB_ICONINFORMATION);
      }

      mpServer->StartServer();
   }
}

//*********************************************************************************************************************
//
// Function: Notify
//
// Description:
//    The system calls this function to obtain the cursor to display while the user drags the minimized windows.
//
// Arguments:
//    N/A
//
// Return:
//    TODO: Add description.
//
//*********************************************************************************************************************
void MainDialog::Notify(Information* thepServerInformation)
{
   if(thepServerInformation->type == Information::Connection)
   {
      CString currentText = L"";
      GetDlgItemText(IDC_LOG_EDIT_BOX, currentText);
      currentText += thepServerInformation->user.c_str();
      currentText += " has connected to the chat.\r\n";
      SetDlgItemText(IDC_LOG_EDIT_BOX, currentText);
   }
   else if (thepServerInformation->type == Information::Message)
   {
      CString currentText = L"";
      GetDlgItemText(IDC_LOG_EDIT_BOX, currentText);
      currentText += thepServerInformation->message.c_str();
      SetDlgItemText(IDC_LOG_EDIT_BOX, currentText);
   }
}