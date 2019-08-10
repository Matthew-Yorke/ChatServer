
#include "stdafx.h"
#include "AboutDialog.h"
#include "afxdialogex.h"
#include "resource.h"

//*********************************************************************************************************************
//
// Function: MainDialog
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
AboutDialog::AboutDialog() : CDialogEx(IDD_ABOUTBOX)
{
}

//*********************************************************************************************************************
//
// Function: DoDataExchange
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
void AboutDialog::DoDataExchange(CDataExchange* thpDialogExchange)
{
	CDialogEx::DoDataExchange(thpDialogExchange);
}

//*********************************************************************************************************************
//
// MACRO: BEGIN_MESSAGE_MAP
//
// Description:
//    This macro maps callbacks for particular actions to functions calls. Currently to about dialog has no callbacks.
//
// Arguments:
//    CChatServerDlg - TODO: Add description.
//    CDialogEx - TODO: Add description.
//
//*********************************************************************************************************************
BEGIN_MESSAGE_MAP(AboutDialog, CDialogEx)
END_MESSAGE_MAP()