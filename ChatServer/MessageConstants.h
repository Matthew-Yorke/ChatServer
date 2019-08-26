//*********************************************************************************************************************
//
// File Name: GuiMessageConstants.h
//
// Description:
//    Holds all the constants for the messages used in the server.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 08/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#include <afx.h>

#ifndef GuiMessageConstants_H
#define GuiMessageConstants_H

namespace GuiMessageConstants
{
   // The string to use for the error message when the listen socket fails to initialize.
   const LPCTSTR INITIALIZE_LISTEN_SOCKET_ERROR = L"Failed to initialize the listener socket.";

   // The string to use for the error message when the socket fails to create.
   const LPCTSTR CREATE_SOCKET_ERROR = L"Failed to create the socket.";
}

#endif // GuiMessageConstants_H