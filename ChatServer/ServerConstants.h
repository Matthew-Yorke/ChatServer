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
   // The successfully output for when two strings are equal.
   const int STRINGS_ARE_EQUAL = 0;

   // The success code for the WSA Startup procedure.
   const int WSA_STARTUP_SUCCESS = 0;

   // The port number the server is using.
   const int SERVER_PORT_NUMBER = 54000;

   // The size of a message buffer for sending and receiving messages.
   const int MESSAGE_BUFFER_SIZE = 8192;

   // The no flag code for the socket send function.
   const int SEND_NO_FLAG = 0;

   // The delimiter to use when parsing strings.
   const char PARSE_DELIMIETER = ',';

   // The string to determine that a "standard message" message type is received from the client.
   const std::string MESSAGE_TYPE_MESSAGE = "message";

   // The string to determine that a "connection request" message type is received from the client.
   const std::string MESSAGE_TYPE_CONNECTION = "connection";

   // The empty string.
   const std::string BLANK_MESSAGE = "";

   // The string to use when acknowledging.
   const std::string ACKNOWLEDGE_CONNECTION = "ACK";

   // The string to use when unacknowledging.
   const std::string UNACKNOWLEDGE_CONNECTION = "NACK";
}

#endif // ServerConstants_H