//*********************************************************************************************************************
//
// File Name: ServerConstants.h
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
#include <string>

#ifndef ServerConstants_H
#define ServerConstants_H

namespace ServerConstants
{
   const int STRINGS_ARE_EQUAL = 0;

   const int WSA_STARTUP_SUCCESS = 0;

   const int SERVER_PORT_NUMBER = 54000;

   const int MESSAGE_BUFFER_SIZE = 8192;

   const int SEND_NO_FLAG = 0;

   const char PARSE_DELIMIETER = ',';

   const std::string MESSAGE_TYPE_MESSAGE = "message";

   const std::string MESSAGE_TYPE_CONNECTION = "connection";

   const std::string BLANK_MESSAGE = "";

   const std::string ACKNOWLEDGE_CONNECTION = "ACK";

   const std::string UNACKNOWLEDGE_CONNECTION = "NACK";
}

#endif // ServerConstants_H