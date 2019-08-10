//*********************************************************************************************************************
//
// File Name: MainDialog.h
//
// Description:
//    TODO: Add description.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 08/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#ifndef MainDialog_H
#define MainDialog_H

#include "Database.h"
#include "Server.h"

class MainDialog : public CDialogEx
{

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif


//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

// Construction

   public:
	
      //***************************************************************************************************************
      //
      // Method: MainDialog
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
      //***************************************************************************************************************
      MainDialog(CWnd* thepParent = nullptr);

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
      virtual ~MainDialog();

	protected:

      //***************************************************************************************************************
      //
      // Macro: DECLARE_MESSAGE_MAP
      //
      // Description:
      //    TODO: Add description.
      //
      // Arguments:
      //    N/A
      //
      //***************************************************************************************************************
      DECLARE_MESSAGE_MAP()

      //***************************************************************************************************************
      //
      // Method: DoDataExchange
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
      //***************************************************************************************************************
	   virtual void DoDataExchange(CDataExchange* thepDataExchange);

      //***************************************************************************************************************
      //
      // Method: OnInitDialog
      //
      // Description:
      //    TODO: Add description.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    TRUE  - TODO: Add description.
      //    FALSE - TODO: Add description.
      //
      //***************************************************************************************************************
      virtual BOOL OnInitDialog();

      //***************************************************************************************************************
      //
      // Method: OnStartButton
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
      afx_msg void OnStartButton();

      //***************************************************************************************************************
      //
      // Method: OnSysCommand
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
      //***************************************************************************************************************
	   afx_msg void OnSysCommand(UINT theID, LPARAM theParameter);

      //***************************************************************************************************************
      //
      // Method: OnPaint
      //
      // Description:
      //    If you add a minimize button to your dialog, you will need the code below to draw the icon. For MFC
      //    applications using the document/view model, this is automatically done for you by the framework.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
	   afx_msg void OnPaint();

      //***************************************************************************************************************
      //
      // Method: OnQueryDragIcon
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
      //***************************************************************************************************************
	   afx_msg HCURSOR OnQueryDragIcon();

   private:

      // There are currently no private methods.

//*********************************************************************************************************************
// End - Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
// Start - Member Variables
//*********************************************************************************************************************

   public:

      // There are currently no public member variables.

   protected:
	
      // TODO: Add description.
      HICON mHIcon;

   private:

      // TODO: Add description.
      Database* mpDatabase;

      // TODO: Add description.
      Server* mpServer;

      // TODO: Add description.
      CString mDatabaseHost;

      // TODO: Add description.
      CString mDatabaseUser;

      // TODO: Add description.
      CString mDatabasePassword;

      // TODO: Add description.
      CString mDatabaseName;

      // TODO: Add description.
      int mPortNumber;

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // ChatServerDialog_H