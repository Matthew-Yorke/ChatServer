//*********************************************************************************************************************
//
// File Name: Server.h
//
// Description:
//    This object handles the overall TCP server functionality. It allows the set up and destruction of the server. The
//    main functionality is to listen for any client connection and use multithreading to handle individual message
//    handling of client commands and messages.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 07/31/2019 | The basic functionality of the server are available. It will allow
//                           |            | listening for client connections and receiving chat messages to be
//                           |            | broadcasted to all connected clients. Functionality will be expanded in the
//                           |            | future.
//
//*********************************************************************************************************************

#ifndef Server_H
#define Server_H

#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <mutex>
#include "Database.h"

#pragma comment (lib, "ws2_32.lib")

class Server
{

//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

   public:

      //***************************************************************************************************************
      //
      // Method: Server
      //
      // Description:
      //    The constructor of the server object that kicks off the initialization of the server by setting up the
      //    listener socket.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      Server();

      //***************************************************************************************************************
      //
      // Method: ~Server
      //
      // Description:
      //    Destructor of the Server object that will join all threads and then close the listener socket, finally
      //    cleaning up windows sockets.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual ~Server();

      //***************************************************************************************************************
      //
      // Method: StartServer
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
      void StartServer();

   protected:

      // There are currently no protected methods.

   private:

      //***************************************************************************************************************
      //
      // Method: InitializeWindowsSocket
      //
      // Description:
      //    Initialize the window socket library.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    True  - Initialization successful.
      //    False - Initialization failed.
      //
      //***************************************************************************************************************
      bool InitializeWindowsSocket();

      //***************************************************************************************************************
      //
      // Method: CreateListenerSocket
      //
      // Description:
      //    Create the socket used for listening for connections.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    True  - Socket creation successful.
      //    False - Socket creation failed.
      //
      //***************************************************************************************************************
      bool CreateListenerSocket();

      //***************************************************************************************************************
      //
      // Method: BindListenerSocket
      //
      // Description:
      //    Binds the socket to the server's IP address and the designated port number.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void BindListenerSocket();

      //***************************************************************************************************************
      //
      // Method: ListenForConnections
      //
      // Description:
      //    Continuously listen for a connection. If a connection comes in, create a new thread to handle the
      //    connection messaging.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void ListenForConnections();

      //***************************************************************************************************************
      //
      // Method: HandleClient
      //
      // Description:
      //    Thread function to handle receiving messages and then calling to broadcast the message back out to the
      //    clients.
      //
      // Arguments:
      //    theClientSocket - The socket that is being handled for this thread.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      
      void HandleClient(SOCKET theClientSocket);

      //***************************************************************************************************************
      //
      // Method: BroadcastSend
      //
      // Description:
      //    Thread function to handle receiving messages and then calling to broadcast the message back out to the
      //     clients.
      //
      // Arguments:
      //    theSendingClient  - The socket of the client that sent the message to be broadcasted so it can be ignored
      //                        for broadcasting.
      //    theBuffer         - The buffer containing the message to be broadcasted back to the clients.
      //    theBystesRecieved - The number of bytes received to be sent back out.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void BroadcastSend(char* theBuffer, int theBystesRecieved);

//*********************************************************************************************************************
// End - Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
// Start - Member Variables
//*********************************************************************************************************************

   public:

      // There are currently no public member variables.

   protected:

      // There are currently no protected member variables.

   private:

      // The socket to listen for connections.
      SOCKET mListeningSocket;

      // Vector containing the list of all active threads.
      std::vector<std::unique_ptr<std::thread>> mClientThreads;

      // Vector containing the list of all connected clients.
      std::vector<SOCKET> mConnectedClients;

      // Mutex to block critical data.
      std::mutex mMutex;

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // Server_H