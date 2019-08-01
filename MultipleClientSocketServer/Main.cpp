//*********************************************************************************************************************
//
// File Name: Main.cpp
//
// Description:
//    This is the entry point of the program that kicks off the chat server and closes the program on completion.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 07/31/2019 | Initial release.
//
//*********************************************************************************************************************

#include "Server.h"

int main()
{
   Server* pChatServer = new Server();
   delete pChatServer;

   return 0;
}